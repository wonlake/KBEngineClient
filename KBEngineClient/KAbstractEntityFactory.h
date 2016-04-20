#pragma once

#include "KEntity.h"

namespace KBEngineClient
{
	class KAbstractEntityFactory
	{
	public:
		KAbstractEntityFactory();
		virtual ~KAbstractEntityFactory();

	public:
		virtual Entity* CreateEntity() = 0;
	};
}
