#pragma once

#include	"KEntity.h"
#include	"KEntitydef.h"
#include	"KMessage.h"
#include	"KMemoryStream.h"

namespace KBEngineClient
{
	class	KAccount : public Entity
	{
	public:
 		void onCreateAccountResult(int retcode)
		{
			std::cout << "Account::onCreateAccountResult:" << retcode << std::endl;		
		};

		virtual void __init__()
		{
			std::cout << "I'm Account!" << std::endl;
			MemoryStream s;
			//baseCall("reqCreateAccountProxy", s);
		}

		void onUpdatePropertys(MemoryStream& s)
		{
			std::cout << "todo update account propertys from server." << std::endl;
		};

		void onRemoteMethodCall( std::string methodname,MemoryStream& s )
		{
			if( methodname == "receiveMsg")
			{
				std::string who;
				std::string msg;
				s.readBlob ( who );
				s.readBlob ( msg );

			}
		}

	};//end class declaration
}