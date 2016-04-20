#pragma once
#include "KAbstractEntityFactory.h"
#include "KEntityMgr.h"

namespace KBEngineClient
{
	template<typename T>
	class KEntityFactory : KAbstractEntityFactory
	{
	private:
		std::string _name;
		
	public:
		KEntityFactory(const std::string& name):_name(name)
		{
			Register();
		}

	public:
		T* CreateEntity() override
		{
			T* t = new T();
			t->classtype = _name;
			return t;
		}
	public:
		void Register()
		{
			KEntityMgr::GetInstance().Register(_name, this );
		}
	};	
}

