//
//  KNetworkInterface.cpp
//  libKBEClient
//
//  Created by Tom on 6/12/14.
//  Copyright (c) 2014 Tom. All rights reserved.
//

#include "KNetworkInterface.h"
#include "KMessage.h"
#include <time.h>

namespace KBEngineClient
{
	bool KNetworkInterface::connect(const std::string& ip, uint16 port)
	{
		return _socket.connect(ip.c_str(), port);
	}

	void KNetworkInterface::send(uint8* data,int size)
	{
		_socket.sendData((char*)data,size);

	}

	void KNetworkInterface::recv()
	{

	}

	void KNetworkInterface::bindMessage()
	{
		
	}

	void KNetworkInterface::process()
	{
		auto tm1 = clock();
		static auto tmLast = clock();

		float dt = (tm1 - tmLast) / (float)CLOCKS_PER_SEC;
		if (dt > 1.f)
		{
			_socket.tick(dt);
			tmLast = tm1;
		}
	}
}