//
//  KNetworkInterface.h
//  libKBEClient
//
//  Created by Tom on 6/12/14.
//  Copyright (c) 2014 Tom. All rights reserved.
//

#ifndef __libKBEClient__KNetworkInterface__
#define __libKBEClient__KNetworkInterface__

#include <iostream>
#include "GameSocket.h"

namespace KBEngineClient
{

	class KNetworkInterface
	{
	public:
		KNetworkInterface()
		{
			reset();
		}

		~KNetworkInterface()
		{
		}

	public:
		typedef void(*ConnectCallBack)();

		struct ConnectState
		{
			std::string connectIP = "";
			int connectPort = 0;
			ConnectCallBack  callback;
			void* userData = nullptr;
			GameSocket* socket = nullptr;
			KNetworkInterface* networkInterface = nullptr;
			std::string error = "";
		};

	public:
		static const int32 TCP_PACKET_MAX = 1460;

	public:
		void reset()
		{
			if (isValid())
			{
				_socket.close();
			}
		}

		bool isValid()
		{
			return _socket.isConnected();
		}

		bool connect(const std::string& ip, uint16 port);
		void send(uint8* data,int size);
		void recv();
		void bindMessage();
		void process();

	private:
		GameSocket _socket;
	};
}
#endif /* defined(__libKBEClient__KNetworkInterface__) */
