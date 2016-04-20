#include "GameSocket.h"
#include "KMessage.h"
#include "KAccount.h"
#include "KBEngineApp.h"
#include "KMessageCallbackMgr.h"

namespace KBEngineClient
{
#define RECV_LEN 4096

	static MemoryStream stream;

	GameSocket::GameSocket()
	{
	}	

	GameSocket::~GameSocket()
	{
		if (sock)
			sock.reset();
	}

	// 消息处理
	void GameSocket::tick(float dt)
	{
		if (sock && m_bConnected)
		{
			char buf[RECV_LEN];
			memset(buf, 0, RECV_LEN);

			std::string msg;
			sock->RecvMsg(msg);
			size_t len = msg.size();

			if (len > 0)
			{
				stream.append(msg.data(), len);

				while (stream.opsize() > 1)
				{
					uint16 msgid;
					uint16 msglen;

					auto msg = KMessage::idxMessages.end();

					if (stream.opsize() >= 2)
					{
						stream >> msgid;
						msg = KMessage::idxMessages.find(msgid);
						if (msg != KMessage::idxMessages.end())
						{
							if (msg->second->msglen == -1)
							{
								if (stream.opsize())
									stream >> msglen;
								else
								{
									std::cout << "warning!!!!!!!!!!!! no msglen data!" << std::endl;
									size_t pos = stream.rpos();
									stream.rpos(pos - 2);
									break;
								}
							}
							else
							{
								msglen = msg->second->msglen;
							}
						}
						else
						{
							std::cout << "warning!!!!!!!!!!!! unknown msgid!" << msgid << "\n";
							stream.clear(true);
							break;
						}
					}
					else
					{
						std::cout << "warning!!!!!!!!!!!!!! no opsize data! " << stream.opsize() << std::endl;	

						break;
					}

					// 检查消息体
					if ((int)stream.opsize() < msglen - 4)
					{
						size_t pos = stream.rpos();
						stream.rpos(pos - 4);
						std::cout << " stream opsize = " << stream.opsize() << std::endl;
						break;
					}
					std::cout << "process msgid = " << msgid << ", msglen = " << msglen << std::endl;
					
					auto& callbackMgr = KMessageCallbackMgr::GetInstance();
					auto callback = callbackMgr.GetCallback(msg->second->msgname);
					if (callback)
					{
						callback(stream);
						stream.safeClear(true);
						continue;
					}

					stream.read_skip(msglen);
					stream.safeClear(true);
					std::cout << "msgid = " << msgid << " be ignored! msglen = " << msglen << std::endl;
				}
			}
			else
			{
				if (!sock->m_bInWorking)
					m_bConnected = false;
			}
		}

		if (sock && m_bConnected)
		{
			handleHeartBeat(dt);
		}
	}

	// 通知服务器的连接断开
	void GameSocket::noticeSocketDisconnect(int len, int sockErr)
	{
		std::cout << "socket disconnected!!!!!!!!!, len: " << len << ", error: " << sockErr << std::endl;
	}

	// 发送心跳包
	void GameSocket::handleHeartBeat(float dt)
	{
		heartBeatTime += dt;
		if (heartBeatTime > HEARTBEAT_INTERVAL)
		{
			KMessage::sendActiveAck();
			heartBeatTime = 0;
		}
	}

	// 连接服务器
	bool GameSocket::connect(const char* host, uint16 port)
	{
		m_strHost = host;
		m_nPort = port;
		BOOL bRet = sock->Start(host, port, FALSE);			
		if (bRet)
			m_bConnected = true;
		
		return m_bConnected;
	}

	// 关闭与服务器的连接
	void GameSocket::close()
	{
		if (sock)
		{
			sock.reset();
			sock = std::make_shared<NetworkService>();
			m_bConnected = false;
		}
	}

	// 发送数据
	void GameSocket::sendData(char* buf, int buflen)
	{
		int sended = 0;

		if (m_bConnected)
		{
			int offset = 0;
			while (buflen > MAX_BUFFER_SIZE)
			{
				sock->SendMsg(buf + offset, MAX_BUFFER_SIZE);
				offset += MAX_BUFFER_SIZE;
				buflen -= MAX_BUFFER_SIZE;
			}
			sock->SendMsg(buf + offset, buflen);
		}		
	}
}