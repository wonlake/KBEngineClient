//
//  KMessage.h
//  libKBEClient
//
//  Created by Tom on 6/12/14.
//  Copyright (c) 2014 Tom. All rights reserved.
//

#ifndef __libKBEClient__KMessage__
#define __libKBEClient__KMessage__

#include "basic_types.h"
#include "KMemoryStream.h"
#include <iostream>

namespace KBEngineClient
{

	typedef uint16 MessageID;
	typedef uint8  Byte ;

	class KMessage{
	
	public:
		KMessage(uint16 msgid, const std::string& msgname,
			int16 length, uint8 argstype, std::list<uint8> msgargtypes, void*);
		KMessage(const KMessage& other);
		KMessage(): id(0), msglen(-1), msgname(), argstype(0)
		{
			std::cout<< "empty message created!" << std::endl;
		};

	public:
		uint16 id = 0;
		std::string msgname = "";
		int16 msglen = -1;
		uint8 argstype = 0;
		std::list<uint8> msgargtypes;
		
		static std::string base_ip;
		static uint16 base_port;

		static std::map<std::string,KMessage*> messages;
		static std::map<uint16,KMessage*> loginappMessages;
		static std::map<uint16,KMessage*> baseappMessages;
		static std::map<uint16,KMessage*> clientMessages;

		static std::map<uint16, KMessage*> idxMessages;

	public:
		static void clear()
		{
			messages.clear();

			loginappMessages.clear();
			baseappMessages.clear();
			clientMessages.clear();

			idxMessages.clear();

			bindFixedMessage();
		}
		static void bindFixedMessage();
		static void onImportClientMessages( MemoryStream& stream );
		static void onImportClientMessagesCompleted();
		static void Client_onImportServerErrorsDescr(MemoryStream& stream);
		static void Client_onVersionNotMatch(MemoryStream& stream);
		static void Client_onLoginSuccessfully(MemoryStream& stream);
		static void onImportEntityDefCompleted();
		static void Client_onLoginGatewayFailed(uint16 failedcode);
		static void onImportClientEntityDef( MemoryStream& stream );
		static void createDataTypeFromStream( MemoryStream& stream, bool canprint );
		static void sendActiveAck();
	}; 

}

#endif /* defined(__libKBEClient__KMessage__) */
