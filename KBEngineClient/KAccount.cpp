#include "KAccount.h"

#include "KEntityFactory.h"

namespace KBEngineClient
{
	KEntityFactory<KAccount>* g_account = new KEntityFactory<KAccount>("Account");
}

