#include "KBEMain.h"


namespace KBEngineClient
{
	KBEMain::KBEMain()
	{
	}


	KBEMain::~KBEMain()
	{
	}

	void KBEMain::InitKBEngine()
	{
		KBEngineArgs args;

		args.ip = ip;
		args.port = port;
		args.clientType = clientType;

		args.persistentDataPath = persistentDataPath;

		args.syncPlayer = syncPlayer;
		args.threadUpdateHZ = threadUpdateHZ;
		args.useAliasEntityID = useAliasEntityID;

		args.SEND_BUFFER_MAX = SEND_BUFFER_MAX;
		args.RECV_BUFFER_MAX = RECV_BUFFER_MAX;

		args.isMultiThreads = isMultiThreads;

		KBEngineApp::create(args);
		gameApp = &KBEngineApp::instance();
	}

	void KBEMain::Tick(float dt)
	{
		gameApp->process();
	}
}