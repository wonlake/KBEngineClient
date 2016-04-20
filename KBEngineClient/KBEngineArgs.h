#pragma once

#include <string>
#include "KNetworkInterface.h"

namespace KBEngineClient
{
	enum CLIENT_TYPE
	{
		// Mobile(Phone, Pad)
		CLIENT_TYPE_MOBILE = 1,

		// Windows Application program
		CLIENT_TYPE_WIN = 2,

		// Linux Application program
		CLIENT_TYPE_LINUX = 3,

		// Mac Application program
		CLIENT_TYPE_MAC = 4,

		// Web£¬HTML5£¬Flash
		CLIENT_TYPE_BROWSER = 5,

		// bots
		CLIENT_TYPE_BOTS = 6,

		// Mini-Client
		CLIENT_TYPE_MINI = 7,
	};

	class KBEngineArgs
	{
	public:
		KBEngineArgs();
		~KBEngineArgs();

	public:
		std::string ip = "127.0.0.1";
		int port = 20013;

		std::string base_ip = "127.0.0.1";
		int base_port = 0;

		enum CLIENT_TYPE clientType = CLIENT_TYPE_MINI;
		std::string persistentDataPath = "";
		bool syncPlayer = true;
		bool useAliasEntityID = true;
		int32 SEND_BUFFER_MAX = KNetworkInterface::TCP_PACKET_MAX;
		int32 RECV_BUFFER_MAX = KNetworkInterface::TCP_PACKET_MAX;

		bool isMultiThreads = false;
		int threadUpdateHZ = 10;

	public:
		int32 getRecvBufferSize()
		{
			return RECV_BUFFER_MAX;
		}

		int32 getSendBufferSize()
		{
			return SEND_BUFFER_MAX;
		}
	};
}

