#pragma once

#include "NetworkService.h"
#include "basic_types.h"
#include <memory>
#define HEARTBEAT_INTERVAL	15

namespace KBEngineClient
{

	class GameSocket
	{
	public:
		GameSocket();
		~GameSocket();

		virtual bool connect(const char* host, uint16 port);
		virtual void close();
		virtual void tick(float dt);
		virtual void sendData(char* data, int size);
		virtual void noticeSocketDisconnect(int len, int sockErr);

		bool isConnected() { return m_bConnected; }

	protected:
		bool m_bConnected = false;
		std::string m_strHost = "127.0.0.1";
		unsigned short m_nPort = 20015;
		std::shared_ptr<NetworkService> sock = std::make_shared<NetworkService>();

	private:
		void handleHeartBeat(float dt);

	private:
		float heartBeatTime = 0.0f;

	};
}