#pragma once

#include <map>
#include <string>
#include <functional>
#include <iostream>

#include "KMemoryStream.h"

namespace KBEngineClient
{
	class KMessageCallbackMgr
	{
	public:
		KMessageCallbackMgr();
		~KMessageCallbackMgr();


	private:
		std::map< std::string, std::function<void(MemoryStream& stream)> > mapCallbacks;

	public:
		void Clear()
		{
			mapCallbacks.clear();
		}

		void Register(const std::string& name, std::function<void(MemoryStream& stream)> newCallback)
		{
			auto callback = mapCallbacks.find(name);
			if (callback != mapCallbacks.end())
			{
				std::cout << "warning!!!!!!!callback will be replaced!" << name << std::endl;
				callback->second = newCallback;
			}
			else
			{
				mapCallbacks.insert(std::make_pair(name, newCallback));
			}
		}

		void Unregister(const std::string& name)
		{
			auto callback = mapCallbacks.find(name);
			if (callback == mapCallbacks.end())
			{
				std::cout << "warning!!!!!!!There is no callback for !" << name << std::endl;
			}
			else
				mapCallbacks.erase(callback);
		}

		std::function<void (MemoryStream&)> GetCallback(const std::string& name)
		{
			auto callback = mapCallbacks.find(name);
			if (callback != mapCallbacks.end())
			{
				return callback->second;
			}
			else
			{
				std::cout << "warning!!!!!!!There is no callback for !" << name << std::endl;
				return std::function<void(MemoryStream&)>();
			}

		}
	
		static KMessageCallbackMgr& GetInstance()
		{
			static KMessageCallbackMgr mgr;
			return mgr;
		}
	};

}