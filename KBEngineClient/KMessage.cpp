//
//  KMessage.cpp
//  libKBEClient
//
//  Created by Tom on 6/12/14.
//  Copyright (c) 2014 Tom. All rights reserved.
//

#include "KMessage.h"
#include "KBundle.h"
#include "KEntitydef.h"
#include "KDataTypes.h"
#include "KMailbox.h"
#include "KBEngineApp.h"
#include "StringEncodingUtility.h"
#include "KMessageCallbackMgr.h"
#include <sstream>

// create static message template.  

namespace KBEngineClient {

	std::map<std::string, KMessage*> KBEngineClient::KMessage::messages;
	std::map<uint16, KMessage*> KBEngineClient::KMessage::loginappMessages;
	std::map<uint16, KMessage*> KBEngineClient::KMessage::baseappMessages;
	std::map<uint16, KMessage*> KBEngineClient::KMessage::clientMessages;

	std::map<uint16, KMessage*> KBEngineClient::KMessage::idxMessages;

	std::string KBEngineClient::KMessage::base_ip;
	uint16 KBEngineClient::KMessage::base_port;

	void KBEngineClient::KMessage::bindFixedMessage()
	{
		KMessage::messages["Loginapp_importClientMessages"] = new KMessage(5, "importClientMessages", 0, 0, std::list<uint8>(), NULL);
		KMessage::messages["Loginapp_hello"] = new KMessage(4, "hello", -1, -1, std::list<uint8>(), NULL);

		KMessage::messages["Baseapp_importClientMessages"] = new KMessage(207, "importClientMessages", 0, 0, std::list<uint8>(), NULL);
		KMessage::messages["Baseapp_importClientEntityDef"] = new KMessage(208, "importClientMessages", 0, 0, std::list<uint8>(), NULL);
		KMessage::messages["Baseapp_hello"] = new KMessage(200, "hello", -1, -1, std::list<uint8>(), NULL);

		KMessage::messages["Client_onHelloCB"] = new KMessage(521, "Client_onHelloCB", -1, -1, std::list<uint8>(), NULL);
		KMessage::clientMessages[KMessage::messages["Client_onHelloCB"]->id] = KMessage::messages["Client_onHelloCB"];

		KMessage::messages["Client_onScriptVersionNotMatch"] = new KMessage(522, "Client_onScriptVersionNotMatch", -1, -1, std::list<uint8>(), NULL);
		KMessage::clientMessages[KMessage::messages["Client_onScriptVersionNotMatch"]->id] = KMessage::messages["Client_onScriptVersionNotMatch"];

		KMessage::messages["Client_onVersionNotMatch"] = new KMessage(523, "Client_onVersionNotMatch", -1, -1, std::list<uint8>(), NULL);
		KMessage::clientMessages[KMessage::messages["Client_onVersionNotMatch"]->id] = KMessage::messages["Client_onVersionNotMatch"];

		KMessage::messages["Client_onImportClientMessages"] = new KMessage(518, "Client_onImportClientMessages", -1, -1, std::list<uint8>(), NULL);
		KMessage::clientMessages[KMessage::messages["Client_onImportClientMessages"]->id] = KMessage::messages["Client_onImportClientMessages"];

		std::for_each(messages.begin(), messages.end(),
			[](std::pair<std::string, KMessage*> msg)
			{
				idxMessages[msg.second->id] = msg.second;
			}
		);

		auto& callbackMgr = KMessageCallbackMgr::GetInstance();
		std::function<void(MemoryStream&)> callback = onImportClientMessages;
		callbackMgr.Register("Client_onImportClientMessages", callback);

		callback = KMessage::onImportClientEntityDef;
		callbackMgr.Register("Client_onImportClientEntityDef", callback);

		callback = Client_onVersionNotMatch;
		callbackMgr.Register("Client_onVersionNotMatch", callback);

		callback = Client_onImportServerErrorsDescr;
		callbackMgr.Register("Client_onImportServerErrorsDescr", callback);

		callback = Client_onLoginSuccessfully;
		callbackMgr.Register("Client_onLoginSuccessfully", callback);
	}


	KBEngineClient::KMessage::KMessage(const KMessage& other)
	{
		this->id = other.id;
		this->msglen = other.msglen;
		this->msgname = other.msgname;
		this->argstype = other.argstype;
		this->msgargtypes = other.msgargtypes;
	}

	KBEngineClient::KMessage::KMessage(uint16 msgid,
		const std::string& msgname, int16 length, uint8 argstype,
		std::list<uint8> msgargtypes, void*) :
		id(msgid),
		msglen(length),
		msgname(msgname),
		argstype(argstype),
		msgargtypes(msgargtypes)
	{
	}

	void KBEngineClient::KMessage::onImportClientMessages(MemoryStream& stream)
	{
		uint16 msgcount;
		stream >> msgcount;

		while (msgcount > 0)
		{
			msgcount--;

			MessageID msgid;
			stream >> msgid;
			int16 msglen;
			stream >> msglen;

			std::string msgname;
			stream >> msgname;

			int8 argstype;
			stream >> argstype;
			Byte argsize;
			stream >> argsize;

			std::list<uint8> argstypes;

			for (Byte i = 0; i < argsize; i++)
			{
				uint8 _tmp;
				stream >> _tmp;
				argstypes.push_back(_tmp);
			}

			bool isClientMethod = msgname.find("Client_") == 0;

			if (isClientMethod)
			{
				std::cout << "here" << "\n";
			}

			if (msgname.length() > 0)
			{
				int handler = 0;
				KMessage::messages[msgname] = new KMessage(msgid, msgname, msglen, argstype, argstypes, 0);
				KMessage::idxMessages[msgid] = KMessage::messages[msgname];
				if (isClientMethod)
				{
					KMessage::clientMessages[msgid] = KMessage::messages[msgname];
					std::cout << "KBEngine::onImportClientMessages store {" << msgname << "} " << std::endl;
				}
				else
				{
					//if(currserver_ == "loginapp")
					if (base_ip == "")
						KMessage::loginappMessages[msgid] = KMessage::messages[msgname];
					else
						KMessage::baseappMessages[msgid] = KMessage::messages[msgname];
				}
			}
			else
			{
				//todo replace handler with real handle call back!
				KMessage* msg = new KMessage(msgid, msgname, msglen, argstype, argstypes, NULL);

				/*	if(!isClientMethod)
						Dbg.DEBUG_MSG(string.Format("KBEngine::onImportClientMessages[{0}]: imported({1}/{2}/{3}) successfully!",
							currserver_, msgname, msgid, msglen));*/
				std::string curserver_ = "loginapp";

				if (base_ip != "")
					curserver_ = "baseapp";
				//
				if (curserver_ == "loginapp")
					KMessage::loginappMessages[msgid] = msg;
				else
					KMessage::baseappMessages[msgid] = msg;
			}
		};

		onImportClientMessagesCompleted();
	}

	void KBEngineClient::KMessage::onImportEntityDefCompleted()
	{
		//done
		KBEngineApp::instance().login_baseapp(false);
	}

	void KBEngineClient::KMessage::onImportClientMessagesCompleted()
	{
		KBEngineClient::KBundle* bundle = new KBundle();
		auto network = KBEngineApp::instance().networkInterface();

		if (base_ip == "")
		{
			//importMercuryErrorDescr
			bundle = new KBundle();
			bundle->newmessage( *KMessage::messages["Loginapp_importServerErrorsDescr"]);
			bundle->send(*network);
			delete bundle;
			//after this  login_loginapp(false);
			
			KBEngineApp::instance().login_loginapp(false);

			/*	bundle = new KBundle();
				bundle->newmessage(*KMessage::messages["Loginapp_login"]);
				int8 clientType = 5;
				bundle->writeInt8(clientType); // clientType
				bundle->writeBlob(new char[0], 0);
				std::string username = "kbe";
				std::string password = "123456";
				bundle->writeString(username);
				bundle->writeString(password);
				bundle->send(*KBEngineApp::instance().networkInterface());
				*/
		}
		else
		{
			//baseapp part.
			bundle = new KBundle();
			bundle->newmessage(*KMessage::messages["Baseapp_importClientEntityDef"]);
			bundle->send(*network);
			//Event.fireOut("Baseapp_importClientEntityDef", new object[]{});
		}
		//createAccount_loginapp(false);
	}

	void KBEngineClient::KMessage::Client_onVersionNotMatch(MemoryStream& stream)
	{
		std::string serverVersion_;
		stream >> serverVersion_;
		std::cout << " serverVersion is " << serverVersion_ << std::endl;

	}

	void KBEngineClient::KMessage::Client_onLoginSuccessfully(MemoryStream& stream)
	{
		std::string accountName;
		stream >> accountName;
		std::string  username = accountName;
		std::string ip;
		stream >> ip;
		uint16 port;
		stream >> port;

		std::string serverdatas_;
		stream.readBlob(serverdatas_);

		std::cout << "KBEngine::Client_onLoginSuccessfully: accountName( " << accountName <<
			" ), addr( " << ip << " : port " << port << "), datas( " << serverdatas_.size() << ")!" << std::endl;

		auto& args = KBEngineApp::instance().getInitArgs();
		args.base_ip = ip;
		args.base_port = port;
		base_ip = ip;
		base_port = port;
		KBEngineApp::instance().login_baseapp(true);
	}

	void KBEngineClient::KMessage::Client_onImportServerErrorsDescr(MemoryStream& stream)
	{
		uint16 size;
		stream >> size;
		while (size > 0)
		{
			size -= 1;

			//MercuryErr e;
			uint16 id;
			stream >> id;
			std::string name;
			std::vector<uint8> datas;
			stream >> datas;
			name.assign(datas.begin(), datas.end());

			std::string descr;
			stream >> datas;
			descr.assign(datas.begin(), datas.end());
			descr = StringEncodingUtility::ws2s(StringEncodingUtility::utf8_2_ws(descr));

			std::cout << "Client_onImportMercuryErrorsDescr: id=" << id << " name=" << name << " descr=" << descr << std::endl;
		}
	}

	void KBEngineClient::KMessage::Client_onLoginGatewayFailed(uint16 failedcode)
	{
		std::cout << "KBEngine::Client_onLoginGatewayFailed: failedcode " << failedcode << std::endl;
	}

	//huge packet be parsed here, store entity define data .
	void KBEngineClient::KMessage::onImportClientEntityDef(MemoryStream& stream)
	{
		//52 
		uint16 aliassize = stream.readUint16();
		std::cout << "KBEngine::Client_onImportClientEntityDef: importAlias(size= " << aliassize << ")!" << std::endl;

		while (aliassize > 0)
		{
			aliassize--;
			createDataTypeFromStream(stream, true);
		};

		//after this, rpos of stream should be changed. if not use & or pointer. stream will never be changed.

		/*	foreach(string datatype in EntityDef.datatypes.Keys)
			{
				if(EntityDef.datatypes[datatype] != null)
				{
					EntityDef.datatypes[datatype].bind();
				}
			}*/
			//when quit loop?
		for (std::map<std::string, KBEDATATYPE_BASE>::iterator it = EntityDef::datatypes.begin(); it != EntityDef::datatypes.end(); it++)
		{
			//if( it->second)
			std::cout << "datatypes  " << it->first << " " << it->second.name << std::endl;
			continue;
			//
			if (it->second.name != "")
				it->second.bind();
			else
				std::cout << "datatypes  " << it->first << " not binded" << std::endl;
		}
		//bind datatype class.

		while (stream.opsize() > 0)
		{
			std::string scriptmethod_name = stream.readString();
			uint16 scriptUtype = stream.readUint16();
			uint16 propertysize = stream.readUint16();
			uint16 methodsize = stream.readUint16();
			uint16 base_methodsize = stream.readUint16();
			uint16 cell_methodsize = stream.readUint16();

			std::cout << "KBEngine::Client_onImportClientEntityDef: import( " 
				<< scriptmethod_name << " ), propertys( " << propertysize << " ) clientMethods( " 
				<< methodsize << " ), baseMethods( " << base_methodsize << " ), cellMethods( " 
				<< cell_methodsize << " )!" << std::endl;

			ScriptModule* module = new ScriptModule(scriptmethod_name);
			//KBEngineClient::EntityDef::moduledefs[scriptmethod_name] = *module;
			//EntityDef::idmoduledefs[scriptUtype] = *module;

			//Dictionary<string, Property> defpropertys = new Dictionary<string, Property>();
			PropertyMap defpropertys;
			EntityDef::alldefpropertys[scriptmethod_name] = &defpropertys;

			//Todo: with cpp version reflection it.
			//Type Class = module.script;

			while (propertysize > 0)
			{
				propertysize--;

				uint16 properUtype = stream.readUint16();
				uint32 properFlags = stream.readUint32();
				int16 ialiasID = stream.readInt16();
				std::string name = stream.readString();
				std::string defaultValStr = stream.readString();
				KBEDATATYPE_BASE& utype = (EntityDef::iddatatypes[stream.readUint16()]);

				//System.Reflection.MethodInfo setmethod = null;

				/*if(Class != null)
				{
					setmethod = Class.GetMethod("set_" + name);
				}*/

				std::string setmethodName = "set_" + name;

				pMethod setmethod = 0;

				Property savedata = *(new Property());
				savedata.name = name;
				savedata.properUtype = properUtype;
				savedata.aliasID = ialiasID;
				savedata.defaultValStr = defaultValStr;
				savedata.utype = utype;
				savedata.setmethod = setmethodName;

				module->propertys[name] = &savedata;

				if (ialiasID >= 0)
				{
					module->usePropertyDescrAlias = true;
					module->idpropertys[(uint16)ialiasID] = &savedata;
				}
				else
				{
					module->usePropertyDescrAlias = false;
					module->idpropertys[properUtype] = &savedata;
				}

				//Dbg.DEBUG_MSG("KBEngine::Client_onImportClientEntityDef: add(" + scriptmethod_name + "), property(" + name + "/" + properUtype + ").");
			};

			while (methodsize > 0)
			{
				methodsize--;

				uint16 methodUtype = stream.readUint16();
				int16 ialiasID = stream.readInt16();
				std::string name = stream.readString();
				Byte argssize = stream.readUint8();
				//List<KBEDATATYPE_BASE> args = new List<KBEDATATYPE_BASE>();
				std::vector<KBEDATATYPE_BASE*> args;
				while (argssize > 0)
				{
					argssize--;
					//args.Add(EntityDef.iddatatypes[stream.readUint16()]);
					args.push_back(&EntityDef::iddatatypes[stream.readUint16()]);
				};

				Method* savedata_ = new Method();
				Method& savedata = *savedata_;
				savedata.name = name;
				savedata.methodUtype = methodUtype;
				savedata.aliasID = ialiasID;
				savedata.args = args;

				/*if(Class != null)
					savedata.handler = Class.GetMethod(name);*/

				module->methods[name] = savedata_;

				if (ialiasID >= 0)
				{
					module->useMethodDescrAlias = true;
					module->idmethods[(uint16)ialiasID] = savedata_;
				}
				else
				{
					module->useMethodDescrAlias = false;
					module->idmethods[methodUtype] = savedata_;
				}

				std::cout << "KBEngine::Client_onImportClientEntityDef: add( " << scriptmethod_name << " ), method( " << name << " )." << std::endl;
			};

			while (base_methodsize > 0)
			{
				base_methodsize--;

				uint16 methodUtype = stream.readUint16();
				int16 ialiasID = stream.readInt16();
				std::string name = stream.readString();
				Byte argssize = stream.readUint8();
				//List<KBEDATATYPE_BASE> args = new List<KBEDATATYPE_BASE>();
				std::vector<KBEDATATYPE_BASE*> args;

				while (argssize > 0)
				{
					argssize--;
					//args.Add(EntityDef.iddatatypes[stream.readUint16()]);
					args.push_back(&EntityDef::iddatatypes[stream.readUint16()]);
				};

				Method* savedata_ = new Method();
				Method& savedata = *savedata_;
				savedata.name = name;
				savedata.methodUtype = methodUtype;
				savedata.aliasID = ialiasID;
				savedata.args = args;

				module->base_methods[name] = savedata_;
				module->idbase_methods[methodUtype] = savedata_;

				std::cout << "KBEngine::Client_onImportClientEntityDef: add( " << scriptmethod_name << " ), base_method( " << name << ")." << std::endl;
			};

			while (cell_methodsize > 0)
			{
				cell_methodsize--;

				uint16 methodUtype = stream.readUint16();
				int16 ialiasID = stream.readInt16();
				std::string name = stream.readString();
				Byte argssize = stream.readUint8();
				//List<KBEDATATYPE_BASE> args = new List<KBEDATATYPE_BASE>();
				std::vector<KBEDATATYPE_BASE*> args;
				while (argssize > 0)
				{
					argssize--;
					//args.Add(EntityDef.iddatatypes[stream.readUint16()]);
					args.push_back(&EntityDef::iddatatypes[stream.readUint16()]);
					//stored a decode handler? 2014-06-27 to confirm it.
				};

				Method* savedata_ = new Method();
				Method& savedata = *savedata_;
				savedata.name = name;
				savedata.methodUtype = methodUtype;
				savedata.aliasID = ialiasID;
				savedata.args = args;

				module->cell_methods[name] = savedata_;
				module->idcell_methods[methodUtype] = savedata_;
				std::cout << "KBEngine::Client_onImportClientEntityDef: add( " << scriptmethod_name << " )" << ", cell_method( " << name << ")." << std::endl;

				//Dbg.DEBUG_MSG("KBEngine::Client_onImportClientEntityDef: add(" + scriptmethod_name + "), cell_method(" + name + ").");
			};

			//if(module.script == null)
			//{
			//	Dbg.ERROR_MSG("KBEngine::Client_onImportClientEntityDef: module(" + scriptmethod_name + ") not found!");
			//}

			//foreach(string name in module.propertys.Keys)
			for (PropertyMapItr it = module->propertys.begin(); it != module->propertys.end(); it++)
			{

				Property infos = *it->second;//module->propertys[name];

				Property newp = *(new Property());
				newp.name = infos.name;
				newp.properUtype = infos.properUtype;
				newp.aliasID = infos.aliasID;
				newp.utype = infos.utype;
				newp.val = infos.utype.parseDefaultValStr(infos.defaultValStr);
				newp.setmethod = infos.setmethod;

				//defpropertys.Add(infos.name, newp);
				defpropertys[infos.name] = &newp;

				//if(module.script != null && module.script.GetMember(name) == null)
				//{
				//	Dbg.ERROR_MSG(scriptmethod_name + "(" + module.script + "):: property(" + name + ") no defined!");
				//}
			};

			//foreach(string name in module.methods.Keys)
			for (std::map<std::string, Method*>::iterator it = module->methods.begin(); it != module->methods.end(); it++)
			{
				Method infos = *it->second; //module->methods[name];

			/*	if(module.script != null && module.script.GetMethod(name) == null)
				{
					Dbg.WARNING_MSG(scriptmethod_name + "(" + module.script + "):: method(" + name + ") no implement!");
				}*/
			};

			//here store 
			KBEngineClient::EntityDef::moduledefs[scriptmethod_name] = *module;
			EntityDef::idmoduledefs[scriptUtype] = *module;
		}

		onImportEntityDefCompleted();
	}

	void KBEngineClient::KMessage::createDataTypeFromStream(MemoryStream& stream, bool canprint)
	{
		//Read Data Alias and stored it.
		uint16 utype = stream.readUint16();
		std::string name = stream.readString();
		std::string valname = stream.readString();
		
		if (valname.size() == 0)
		{
			std::ostringstream oss;
			oss << utype;
			valname = "Null_" + oss.str();
		}

		if (canprint)
			std::cout << "KBEngine::Client_onImportClientEntityDef: importAlias( " << name << " : " << valname << " )! " << std::endl;
		
		if (name == "FIXED_DICT")
		{
			//AvatarInfo need this data type.
			KBEDATATYPE_FIXED_DICT datatype = *(new KBEDATATYPE_FIXED_DICT());
			Byte keysize = stream.readUint8();
			datatype.implementedBy = stream.readString();
			datatype.name = valname;
			while (keysize > 0)
			{
				keysize--;

				std::string keyname = stream.readString();
				uint16 keyutype = stream.readUint16();
				datatype.dicttype[keyname] = keyutype;
				std::cout << "EntityDef::datatypes key " << keyname << " stored  " << keyutype << std::endl;

			};
			//here is to store a data 
			//KBEDATATYPE_FIXED_DICT d =  datatype;
			EntityDef::datatypes[name] = (KBEDATATYPE_BASE*)&datatype; //missed something?
			std::cout << "EntityDef::datatypes " << name << " stored  type= " << datatype.name << std::endl;
		}
		else if (name == "ARRAY")
		{
			uint16 uitemtype = stream.readUint16();
			KBEDATATYPE_ARRAY datatype = *(new KBEDATATYPE_ARRAY());
			//datatype.type = uitemtype;
			datatype.name = valname;
			EntityDef::datatypes[name] = (KBEDATATYPE_BASE*)&datatype;
		}
		else
		{
			
		}

		EntityDef::iddatatypes[utype] = EntityDef::datatypes[name];
		EntityDef::datatype2id[name] = EntityDef::datatype2id[valname];
	}

	void KBEngineClient::KMessage::sendActiveAck()
	{

		std::map<std::string, KMessage*>::iterator it = KMessage::messages.find("Loginapp_onClientActiveTick");
		//
		std::map<std::string, KMessage*>::iterator it2 = KMessage::messages.find("Baseapp_onClientActiveTick");

		KBundle* bundle = new KBundle();
		if (base_ip == "") //currserver_ == "loginapp")
		{
			if (it == KMessage::messages.end())
				return;
			bundle->newmessage(*KMessage::messages["Loginapp_onClientActiveTick"]);
		}
		else
		{
			if (it2 == KMessage::messages.end())
				return;
			bundle->newmessage(*KMessage::messages["Baseapp_onClientActiveTick"]);
		}

		bundle->send(*KBEngineApp::instance().networkInterface());
		delete bundle;
	}
}