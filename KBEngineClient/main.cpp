#include <thread>
#include <iostream>
#include "KBEngineApp.h"

int main()
{
	//KBEGameSocket& nc = KBEGameSocket::getInstance();
	//bool bOk = nc.connectionServer("192.168.1.128", 20013);
	//nc.connect("127.0.0.1", 9999);
	using namespace KBEngineClient;
	
	KBEngineArgs args;

	args.ip					= "192.168.1.132";
	args.port				= 20013;
	args.clientType			= CLIENT_TYPE::CLIENT_TYPE_MINI;
	args.persistentDataPath = "Application.persistentDataPath";
	args.syncPlayer			= true;
	args.threadUpdateHZ		= 10;
	args.useAliasEntityID	= true;
	args.SEND_BUFFER_MAX	= KNetworkInterface::TCP_PACKET_MAX;
	args.RECV_BUFFER_MAX	= KNetworkInterface::TCP_PACKET_MAX;
	args.isMultiThreads		= false;

	KBEngineApp::create(args);
	KBEngineApp& app = KBEngineApp::instance();
	app.login("test", "123456", std::string("kbeplugin"));

	if (!app.networkInterface()->isValid())
		return -1;

	bool bExit = false;

	std::thread t1([&]()
	{
		while (!bExit)
		{
			app.process();
		}
	});
	
	//t2.join();
	while (true)
	{
		std::string content;
		std::cin >> content;
		if (content == "quit")
		{
			bExit = true;
			break;
		}
		else if (content == "reqRole")
		{
			std::cout << "reqRole" << std::endl;
			MemoryStream s;
			s << (int)100001;
			s << "wonderful";
			s << (char)1;
			s << (int)100001;
			app._entities[app._entity_id]->baseCall("reqCreateRole", s);
		}
	}
	
	t1.join();
	return 0;
}