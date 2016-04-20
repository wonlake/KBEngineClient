#pragma once

#include <memory>
#include <time.h>

#include "KNetworkInterface.h"
#include "KDataTypes.h"
#include "KEntity.h"
#include "KBEngineArgs.h"

namespace KBEngineClient
{
	class KBEngineApp
	{
	public:
		KBEngineApp() = delete;
		~KBEngineApp();

	private:
		static KBEngineApp* app;

	public:
		std::string _username = "kbe";
		std::string _password = "123456";

		bool loadingLocalMessages_ = false;

		bool loginappMessageImported_ = false;
		bool baseappMessageImported_ = false;
		bool entitydefImported_ = false;
		bool isImportServerErrorsDescr_ = false;

		std::string _currserver = "";
		std::string _currstate = "";

		std::string _serverdatas = "";
		std::string _clientdatas = "";
		std::string _encryptedKey = "";

		std::string _serverVersion = "";
		std::string _clientVersion = "0.8.0";
		std::string _serverScriptVersion = "";
		std::string _clientScriptVersion = "0.1.0";
		std::string _serverProtocolMD5 = "";
		std::string _serverEntitydefMD5 = "";

		// 当前玩家的实体id与实体类别
		uint64 _entity_uuid = 0;
		int32 _entity_id = 0;
		std::string _entity_type = "";

		// 当前玩家最后一次同步到服务端的位置与朝向与
		// 服务端最后一次同步过来的位置

		Vector3 _entityLastLocalPos = { 0, 0, 0 };
		Vector3 _entityLastLocalDir = { 0, 0, 0 };
		Vector3 _entityServerPos = { 0, 0, 0 };

		std::map<int32, Entity*> _entities;
		std::list<int32> _entityIDAliasIDList;
		std::map<int32, MemoryStream> _bufferedCreateEntityMessage;

		struct ServerErr
		{
			std::string _name;
			std::string _descr;
			uint16 _id;
		};

		std::map<uint16, ServerErr> _serverErrs;
		clock_t _lastticktime = clock();
		clock_t _lastUpdateToServerTime = clock();

		uint32 _spaceID = 0;
		std::string _spaceResPath = "";
		bool _isLoadedGeometry = false;

		EntityDef _entityDef;

		const std::string _component = "client";

		std::shared_ptr<KNetworkInterface> _networkInterface;

		KBEngineArgs args_;

	private:
		KBEngineApp(const KBEngineArgs& args);

	public:

		static bool create(const KBEngineArgs& args);

		static KBEngineApp& instance();

		bool initialize(const KBEngineArgs& args);

	public:

		KBEngineArgs& getInitArgs();

		void login(std::string username, std::string password, std::string& datas);

		void clearEntities(bool isall);

		void reset();

		void hello();

		void Client_onHelloCB(MemoryStream& stream);

		void login_loginapp(bool noconnect);

		void login_baseapp(bool noconnect);

		void Client_onCreatedProxies(MemoryStream& stream);

		void initNetwork();

		void process();

		std::shared_ptr<KNetworkInterface>& networkInterface();

		std::string& serverdatas();

		std::string& clientdatas();

		void entityServerPos(const Vector3& pos);

		void resetMessages();

		void Client_onEntityDestroyed(MemoryStream& stream);

		void Client_onRemoteMethodCall(MemoryStream& stream);
	};
}
