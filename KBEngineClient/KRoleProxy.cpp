#include "KRoleProxy.h"

#include "KEntityFactory.h"

namespace KBEngineClient
{
	KEntityFactory<KRoleProxy>* g_roleProxy = new KEntityFactory<KRoleProxy>("RoleProxy");
}

