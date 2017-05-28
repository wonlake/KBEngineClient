#pragma once

#include "KEntity.h"
#include "StringEncodingUtility.h"

namespace KBEngineClient
{
	class KRoleProxy : public Entity
	{
	public:
		void __init__()
		{
			std::cout << "I'm RoleProxy!" << std::endl;
		}

		void onRemoteMethodCall(std::string methodname, MemoryStream& s) override
		{
			if (methodname == "helloFromClient")
			{
				std::string info;
				s.readBlob(info);
				std::cout << "Good news, it's ok!" << StringEncodingUtility::ws2s(StringEncodingUtility::utf8_2_ws(info)) << std::endl;
			}
			if (methodname == "onTest")
			{
				std::cout << "shit!!!!" << std::endl;
			}
			else
			{
				std::cout << "oh no, please check it!" << std::endl;
			}
		}
	};
}