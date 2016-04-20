#include "KEntity.h"
#include "KBEngineApp.h"


bool KBEngineClient::Entity::isPlayer()
{
	return id == KBEngineApp::instance()._entity_id;
}
