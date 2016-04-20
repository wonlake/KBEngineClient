#pragma once

#include "KAbstractEntityFactory.h"

namespace KBEngineClient
{
	class KEntityMgr
	{
	public:
		KEntityMgr();
		~KEntityMgr();

	public:
		static KEntityMgr& GetInstance()
		{
			static KEntityMgr mgr;
			return mgr;
		}

		std::shared_ptr<KAbstractEntityFactory> GetFactory(const std::string& name)
		{
			auto f = mapEntityFactories.find(name);
			if (f != mapEntityFactories.end())
			{
				return f->second;
			}
			else
			{
				std::cout << "warning>>>>>>>> there is no factory named :" << name << std::endl;
				return std::shared_ptr<KAbstractEntityFactory>();
			}
		}

		void Register(const std::string& name, KAbstractEntityFactory* pFactory)
		{
			auto f = mapEntityFactories.find(name);
			if (f != mapEntityFactories.end())
			{
				std::cout << "factory exists already!!!" << name << std::endl;
			}
			else
			{
				mapEntityFactories.insert(std::make_pair(name, std::shared_ptr<KAbstractEntityFactory>(pFactory)));
			}
		}

		std::map<std::string, std::shared_ptr<KAbstractEntityFactory> > mapEntityFactories;


	};
}

