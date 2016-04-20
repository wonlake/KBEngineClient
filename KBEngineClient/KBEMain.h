#pragma once

#include <string>
#include "KBEngineApp.h"
#include "KBEngineArgs.h"

namespace KBEngineClient
{ 
	class KBEMain
	{
	public:
		KBEMain();
		~KBEMain();

	public:
		KBEngineApp* gameApp = nullptr;
		std::string ip = "127.0.0.1";
		int port = 20013;
		enum CLIENT_TYPE clientType = CLIENT_TYPE::CLIENT_TYPE_MINI;
		std::string persistentDataPath = "Application.persistentDataPath";
		bool syncPlayer = true;
		int threadUpdateHZ = 10;
		int32 SEND_BUFFER_MAX = KNetworkInterface::TCP_PACKET_MAX;
		int32 RECV_BUFFER_MAX = KNetworkInterface::TCP_PACKET_MAX;
		bool useAliasEntityID = true;
		bool isMultiThreads = false;

	public:
		void InitKBEngine();

		void Tick(float dt);
	};
}


