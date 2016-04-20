#include "KBEngineApp.h"
#include "KAccount.h"
#include "KMessageCallbackMgr.h"
#include "KEntityMgr.h"

namespace KBEngineClient
{
	KBEngineApp* KBEngineApp::app = nullptr;

	KBEngineApp::KBEngineApp(const KBEngineArgs& args)
	{
		if (app != nullptr)
			return;

		app = this;
		initialize(args);
	}

	KBEngineApp::~KBEngineApp()
	{
	}

	bool KBEngineApp::create(const KBEngineArgs& args)
	{
		if (app != nullptr)
			return false;

		static KBEngineApp engineApp(args);
		app = &engineApp;

		return true;
	}

	KBEngineApp& KBEngineApp::instance()
	{
		return *app;
	}

	bool KBEngineApp::initialize(const KBEngineArgs& args)
	{
		args_ = args;
		initNetwork();
		
		auto& callbackMgr = KMessageCallbackMgr::GetInstance();
		std::function<void(MemoryStream&)> callback = std::bind(&KBEngineApp::Client_onHelloCB, this, std::placeholders::_1);
		callbackMgr.Register("Client_onHelloCB", callback);

		callback = std::bind(&KBEngineApp::Client_onCreatedProxies, this, std::placeholders::_1);
		callbackMgr.Register("Client_onCreatedProxies", callback);

		callback = std::bind(&KBEngineApp::Client_onEntityDestroyed, this, std::placeholders::_1);
		callbackMgr.Register("Client_onEntityDestroyed", callback);
		
		callback = std::bind(&KBEngineApp::Client_onRemoteMethodCall, this, std::placeholders::_1);
		callbackMgr.Register("Client_onRemoteMethodCall", callback);
	
		return true;
	}

	
	void KBEngineApp::Client_onEntityDestroyed(MemoryStream& stream)
	{
		ENTITY_ID eid;
		stream >> eid;
	}

	void KBEngineApp::Client_onRemoteMethodCall(MemoryStream& stream)
	{
		ENTITY_ID eid = 0;
		stream >> eid;

		KBEngineClient::Entity* entity = KBEngineApp::instance()._entities.find(eid)->second;
		uint16 methodUtype = 0;

		uint8  methodUtype1 = 0;
		if (EntityDef::moduledefs[entity->classtype].useMethodDescrAlias)
			//methodUtype1 = stream.readUint8();
			stream >> methodUtype1;
		else
			//methodUtype = stream.readUint16();
			stream >> methodUtype;

		if (methodUtype1 != 0)
			methodUtype = methodUtype1;
		//

		Method& methoddata = *EntityDef::moduledefs[entity->classtype].idmethods[methodUtype];

		std::cout << "KBEngine::Client_onRemoteMethodCall: " << entity->classtype <<
			", " << methoddata.name << std::endl;

		entity->onRemoteMethodCall(methoddata.name, stream);
	}

	KBEngineArgs& KBEngineApp::getInitArgs()
	{
		return args_;
	}

	void KBEngineApp::login(std::string username, std::string password, std::string& datas)
	{
		_username = std::move(username);
		_password = std::move(password);
		_clientdatas = std::move(datas);

		login_loginapp(true);
	}

	void KBEngineApp::clearEntities(bool isall)
	{
		if (!isall)
		{
			std::for_each(_entities.begin(), _entities.end(), [](std::pair<int32, Entity*> e) { delete e.second; });
			_entities.clear();
		}
	}

	void KBEngineApp::reset()
	{
		clearEntities(true);

		_currserver = "";
		_currstate = "";
		_serverdatas = "";
		_serverVersion = "";
		_serverScriptVersion = "";

		_entity_uuid = 0;
		_entity_id = 0;
		_entity_type = "";

		_entityIDAliasIDList.clear();
		_bufferedCreateEntityMessage.clear();

		_lastticktime = clock();
		_lastUpdateToServerTime = clock();

		_spaceID = 0;
		_spaceResPath = "";
		_isLoadedGeometry = false;

		_networkInterface.reset();
		_networkInterface =
			std::make_shared<KNetworkInterface>();
	}

	void KBEngineApp::hello()
	{
		std::shared_ptr<KBundle> bundle = std::make_shared<KBundle>();

		if (_currserver == "loginapp")
		{
			bundle->newmessage(*KMessage::messages["Loginapp_hello"]);
			std::cout << "Post a request : Loginapp_hello" << std::endl;
		}
		else
		{
			bundle->newmessage(*KMessage::messages["Baseapp_hello"]);
			std::cout << "Post a request : Baseapp_hello" << std::endl;
		}

		bundle->writeString(_clientVersion);
		bundle->writeString(_clientScriptVersion);
		bundle->writeBlob(const_cast<char*>(_encryptedKey.data()), _encryptedKey.size());
		bundle->send(*_networkInterface);
	}

	void KBEngineApp::Client_onHelloCB(MemoryStream& stream)
	{
		std::string serverVersion_;
		std::string scriptVersion_;
		std::string digestStr;
		std::string md5Str;

		stream >> serverVersion_;
		stream >> scriptVersion_;
		stream >> digestStr;
		stream >> md5Str;
		int32 ctype;
		stream >> ctype;
		//std::cout << "KBEngine::Client_onHelloCB: verInfo(" << serverVersion_ << "), ctype( " << ctype << ")" << std::endl;
		std::cout << "Get a Response : Client_onHelloCB!" << std::endl;

		if (_currserver == "loginapp")
		{
			std::shared_ptr<KBundle> bundle = std::make_shared<KBundle>();
			bundle->newmessage(*KMessage::messages["Loginapp_importClientMessages"]);
			bundle->send(*KBEngineApp::instance().networkInterface());
			std::cout << "Post a Request : Loginapp_importClientMessages!" << std::endl;
		}
		else
		{
			std::shared_ptr<KBundle> bundle = std::make_shared<KBundle>();
			bundle->newmessage(*KMessage::messages["Baseapp_importClientMessages"]);
			bundle->send(*KBEngineApp::instance().networkInterface());
			std::cout << "Post a Request : Baseapp_importClientMessages!" << std::endl;
		}

	}

	void KBEngineApp::login_loginapp(bool noconnect)
	{
		if (noconnect)
		{
			reset();
			_networkInterface->connect(args_.ip, args_.port);
			if (_networkInterface->isValid())
			{
				_currserver = "loginapp";
				_currstate = "login";

				hello();
			}
		}
		else
		{
			std::shared_ptr<KBundle> bundle = std::make_shared<KBundle>();
			bundle->newmessage(*KMessage::messages["Loginapp_login"]);
			bundle->writeInt8((int8)args_.clientType);
			bundle->writeBlob(const_cast<char*>(_clientdatas.data()), _clientdatas.size() + 1);
			bundle->writeString(_username);
			bundle->writeString(_password);
			bundle->send(*_networkInterface);

			std::cout << "Post a Request with connected : Loginapp_login!" << std::endl;
		}
	}

	void KBEngineApp::login_baseapp(bool noconnect)
	{
		if (noconnect)
		{
			reset();
			_networkInterface->connect(args_.base_ip, args_.base_port);
			if (_networkInterface->isValid())
			{
				_currserver = "baseapp";
				_currstate = "";

				hello();
			}
		}
		else
		{
			std::shared_ptr<KBundle> bundle = std::make_shared<KBundle>();
			bundle->newmessage(*KMessage::messages["Baseapp_loginBaseapp"]);
			bundle->writeString(_username);
			bundle->writeString(_password);
			bundle->send(*_networkInterface);

			std::cout << "Post a Request with connected : Baseapp_login!" << std::endl;
		}
	}

	void KBEngineApp::Client_onCreatedProxies(MemoryStream& stream)
	{
		uint64 uuid;
		int32 eid;
		std::string entityType;

		stream >> uuid;
		stream >> eid;
		stream >> entityType;
		
		std::cout << "^^^^^^^^^^^^^^^^^uuid=" << uuid << ", eid=" << eid << ", entityType=" << entityType << std::endl;

		Entity* pNewEntity = nullptr;
		
		auto entity = _entities.find(eid);
		if (entity != _entities.end())
		{
			delete entity->second;
			entity->second = nullptr;
		}

		auto factory = KEntityMgr::GetInstance().GetFactory(entityType);
		if (factory)
		{
			_entity_id = eid;
			_entity_type = entityType;
			_entity_uuid = uuid;

			pNewEntity = factory->CreateEntity();
			pNewEntity->baseMailbox = new MailBox();
			pNewEntity->baseMailbox->id = eid;
			pNewEntity->baseMailbox->classtype = entityType;
			pNewEntity->baseMailbox->type = MailBox::MAILBOX_TYPE_BASE;
			_entities[eid] = pNewEntity;
			pNewEntity->__init__();
		}
	}

	void KBEngineApp::initNetwork()
	{
		KMessage::bindFixedMessage();
		EntityDef::EntityDef_Bind();

		_networkInterface = std::make_shared<KNetworkInterface>();
	}

	void KBEngineApp::process()
	{
		_networkInterface->process();
	}

	std::shared_ptr<KNetworkInterface>& KBEngineApp::networkInterface()
	{
		return _networkInterface;
	}

	std::string& KBEngineApp::serverdatas()
	{
		return _serverdatas;
	}

	std::string& KBEngineApp::clientdatas()
	{
		return _clientdatas;
	}

	void KBEngineApp::entityServerPos(const Vector3& pos)
	{
		_entityServerPos = pos;
	}

	void KBEngineApp::resetMessages()
	{
		loadingLocalMessages_ = false;
		loginappMessageImported_ = false;
		baseappMessageImported_ = false;
		entitydefImported_ = false;
		isImportServerErrorsDescr_ = false;
		_serverErrs.clear();
	}
}
