//
//  KNetworkInterface.cpp
//  libKBEClient
//
//  Created by Tom on 6/12/14.
//  Copyright (c) 2014 Tom. All rights reserved.
//
 
#ifndef __CLIENT_INTERFACE_MACRO_H__
#define __CLIENT_INTERFACE_MACRO_H__

// common include	
//#include "network/interface_defs.hpp"
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace  KBEngineClient {

#define CLIENTAPP ClientApp
#define CLIENT 1
#define DEFINE_IN_INTERFACE 1
/**
	Client��Ϣ�꣬  ����Ϊ���� ��Ҫ�Լ��⿪
*/
#if defined(NETWORK_INTERFACE_DECLARE_BEGIN)
	#undef CLIENT_MESSAGE_HANDLER_STREAM
#endif

#if defined(DEFINE_IN_INTERFACE)
#if defined(CLIENT)
#define CLIENT_MESSAGE_HANDLER_STREAM(NAME)										\
	void NAME##ClientMessagehandler_stream::handle(,	\
													MemoryStream& s)	\
	{																			\
			KBEngineClient::CLIENTAPP::getInstance().NAME( s);				\
	}																			\

#else
#define CLIENT_MESSAGE_HANDLER_STREAM(NAME)										\
	void NAME##ClientMessagehandler_stream::handle(,	\
													MemoryStream& s)	\
	{																			\
	}																			\
		
#endif
#else
#define CLIENT_MESSAGE_HANDLER_STREAM(NAME)										\
	class NAME##ClientMessagehandler_stream : public Mercury::MessageHandler	\
	{																			\
	public:																		\
		virtual void handle(,							\
							MemoryStream& s);							\
	};																			\

#endif

#define CLIENT_MESSAGE_DECLARE_STREAM(NAME, MSG_LENGTH)							\
	CLIENT_MESSAGE_HANDLER_STREAM(NAME)											\
	NETWORK_MESSAGE_DECLARE_STREAM(Client, NAME,								\
				NAME##ClientMessagehandler_stream, MSG_LENGTH)					\
																				\

/**
	Client��Ϣ�꣬  ֻ�������������Ϣ
*/
#if defined(NETWORK_INTERFACE_DECLARE_BEGIN)
	#undef CLIENT_MESSAGE_HANDLER_ARGS0
#endif

#if defined(DEFINE_IN_INTERFACE)
#if defined(CLIENT)
#define CLIENT_MESSAGE_HANDLER_ARGS0(NAME)										\
	void NAME##ClientMessagehandler0::handle(,		\
												MemoryStream& s)		\
	{																			\
			KBEngineClient::CLIENTAPP::getInstance().NAME(pChannel);					\
	}																			\

#else
#define CLIENT_MESSAGE_HANDLER_ARGS0(NAME)										\
	void NAME##ClientMessagehandler0::handle(,		\
												MemoryStream& s)		\
	{																			\
	}																			\
		
#endif
#else
#define CLIENT_MESSAGE_HANDLER_ARGS0(NAME)										\
	class NAME##ClientMessagehandler0 : public Mercury::MessageHandler			\
	{																			\
	public:																		\
		virtual void handle(,							\
												MemoryStream& s);		\
	};																			\

#endif

#define CLIENT_MESSAGE_DECLARE_ARGS0(NAME, MSG_LENGTH)							\
	CLIENT_MESSAGE_HANDLER_ARGS0(NAME)											\
	NETWORK_MESSAGE_DECLARE_ARGS0(Client, NAME,									\
				NAME##ClientMessagehandler0, MSG_LENGTH)						\
																				\

/**
	Client��Ϣ�꣬  ֻ�ж�����������Ϣ
*/
#if defined(NETWORK_INTERFACE_DECLARE_BEGIN)
	#undef CLIENT_MESSAGE_HANDLER_ARGS2
#endif

#if defined(DEFINE_IN_INTERFACE)
#if defined(CLIENT)
#define CLIENT_MESSAGE_HANDLER_ARGS2(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2)				\
	void NAME##ClientMessagehandler2::handle(,		\
												MemoryStream& s)		\
	{																			\
			ARG_TYPE1 ARG_NAME1;												\
			s >> ARG_NAME1;														\
			ARG_TYPE2 ARG_NAME2;												\
			s >> ARG_NAME2;														\
			KBEngineClient::CLIENTAPP::getInstance().NAME(					\
													ARG_NAME1, ARG_NAME2);		\
	}																			\

#else
#define CLIENT_MESSAGE_HANDLER_ARGS2(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2)				\
	void NAME##ClientMessagehandler2::handle(,		\
												MemoryStream& s)		\
	{																			\
	}																			\
		
#endif
#else
#define CLIENT_MESSAGE_HANDLER_ARGS2(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2)				\
	class NAME##ClientMessagehandler2 : public Mercury::MessageHandler			\
	{																			\
	public:																		\
		virtual void handle(,							\
							MemoryStream& s);							\
	};																			\

#endif

#define CLIENT_MESSAGE_DECLARE_ARGS2(NAME, MSG_LENGTH, ARG_TYPE1, ARG_NAME1,	\
											ARG_TYPE2, ARG_NAME2)				\
	CLIENT_MESSAGE_HANDLER_ARGS2(NAME, ARG_TYPE1, ARG_NAME1, 					\
											ARG_TYPE2, ARG_NAME2)				\
	NETWORK_MESSAGE_DECLARE_ARGS2(Client, NAME,									\
				NAME##ClientMessagehandler2, MSG_LENGTH, ARG_TYPE1, ARG_NAME1,	\
											ARG_TYPE2, ARG_NAME2)				\


/**
	Client��Ϣ�꣬  ֻ��һ����������Ϣ
*/
#if defined(NETWORK_INTERFACE_DECLARE_BEGIN)
	#undef CLIENT_MESSAGE_HANDLER_ARGS1
#endif

#if defined(DEFINE_IN_INTERFACE)
#if defined(CLIENT)
#define CLIENT_MESSAGE_HANDLER_ARGS1(NAME, ARG_TYPE1, ARG_NAME1)				\
	void NAME##ClientMessagehandler1::handle(,		\
												MemoryStream& s)		\
	{																			\
			ARG_TYPE1 ARG_NAME1;												\
			s >> ARG_NAME1;														\
			KBEngineClient::CLIENTAPP::getInstance().NAME( ARG_NAME1);		\
	}																			\

#else
#define CLIENT_MESSAGE_HANDLER_ARGS1(NAME, ARG_TYPE1, ARG_NAME1)				\
	void NAME##ClientMessagehandler1::handle(,		\
												MemoryStream& s)		\
	{																			\
	}																			\
		
#endif
#else
#define CLIENT_MESSAGE_HANDLER_ARGS1(NAME, ARG_TYPE1, ARG_NAME1)				\
	class NAME##ClientMessagehandler1 : public Mercury::MessageHandler			\
	{																			\
	public:																		\
		virtual void handle(,							\
							MemoryStream& s);							\
	};																			\

#endif

#define CLIENT_MESSAGE_DECLARE_ARGS1(NAME, MSG_LENGTH, ARG_TYPE1, ARG_NAME1)	\
	CLIENT_MESSAGE_HANDLER_ARGS1(NAME, ARG_TYPE1, ARG_NAME1)					\
	NETWORK_MESSAGE_DECLARE_ARGS1(Client, NAME,									\
				NAME##ClientMessagehandler1, MSG_LENGTH, ARG_TYPE1, ARG_NAME1)	\
																				\

/**
	Client��Ϣ�꣬  ֻ��������������Ϣ
*/
#if defined(NETWORK_INTERFACE_DECLARE_BEGIN)
	#undef CLIENT_MESSAGE_HANDLER_ARGS3
#endif

#if defined(DEFINE_IN_INTERFACE)
#if defined(CLIENT)
#define CLIENT_MESSAGE_HANDLER_ARGS3(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3)				\
	void NAME##ClientMessagehandler3::handle(,		\
												MemoryStream& s)		\
	{																			\
			ARG_TYPE1 ARG_NAME1;												\
			s >> ARG_NAME1;														\
			ARG_TYPE2 ARG_NAME2;												\
			s >> ARG_NAME2;														\
			ARG_TYPE3 ARG_NAME3;												\
			s >> ARG_NAME3;														\
			KBEngineClient::CLIENTAPP::getInstance().NAME(					\
				ARG_NAME1, ARG_NAME2,										 	\
				ARG_NAME3);														\
	}																			\

#else
#define CLIENT_MESSAGE_HANDLER_ARGS3(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3)				\
	void NAME##ClientMessagehandler3::handle(,		\
												MemoryStream& s)		\
	{																			\
	}																			\
		
#endif
#else
#define CLIENT_MESSAGE_HANDLER_ARGS3(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3)				\
	class NAME##ClientMessagehandler3 : public Mercury::MessageHandler			\
	{																			\
	public:																		\
		virtual void handle(,							\
							MemoryStream& s);							\
	};																			\

#endif

#define CLIENT_MESSAGE_DECLARE_ARGS3(NAME, MSG_LENGTH, ARG_TYPE1, ARG_NAME1,	\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3)				\
	CLIENT_MESSAGE_HANDLER_ARGS3(NAME, ARG_TYPE1, ARG_NAME1, 					\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3)				\
	NETWORK_MESSAGE_DECLARE_ARGS3(Client, NAME,									\
				NAME##ClientMessagehandler3, MSG_LENGTH, ARG_TYPE1, ARG_NAME1,	\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3)				\



/**
	Client��Ϣ�꣬  ֻ���ĸ���������Ϣ
*/
#if defined(NETWORK_INTERFACE_DECLARE_BEGIN)
	#undef CLIENT_MESSAGE_HANDLER_ARGS4
#endif

#if defined(DEFINE_IN_INTERFACE)
#if defined(CLIENT)
#define CLIENT_MESSAGE_HANDLER_ARGS4(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4)				\
	void NAME##ClientMessagehandler4::handle(,		\
												MemoryStream& s)		\
	{																			\
			ARG_TYPE1 ARG_NAME1;												\
			s >> ARG_NAME1;														\
			ARG_TYPE2 ARG_NAME2;												\
			s >> ARG_NAME2;														\
			ARG_TYPE3 ARG_NAME3;												\
			s >> ARG_NAME3;														\
			ARG_TYPE4 ARG_NAME4;												\
			s >> ARG_NAME4;														\
			KBEngineClient::CLIENTAPP::getInstance().NAME(					\
				ARG_NAME1, ARG_NAME2,										 	\
				ARG_NAME3, ARG_NAME4);											\
	}																			\

#else
#define CLIENT_MESSAGE_HANDLER_ARGS4(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4)				\
	void NAME##ClientMessagehandler4::handle(,		\
												MemoryStream& s)		\
	{																			\
	}																			\
		
#endif
#else
#define CLIENT_MESSAGE_HANDLER_ARGS4(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4)				\
	class NAME##ClientMessagehandler4 : public Mercury::MessageHandler			\
	{																			\
	public:																		\
		virtual void handle(,							\
							MemoryStream& s);							\
	};																			\

#endif

#define CLIENT_MESSAGE_DECLARE_ARGS4(NAME, MSG_LENGTH, ARG_TYPE1, ARG_NAME1,	\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4)				\
	CLIENT_MESSAGE_HANDLER_ARGS4(NAME, ARG_TYPE1, ARG_NAME1, 					\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4)				\
	NETWORK_MESSAGE_DECLARE_ARGS4(Client, NAME,									\
				NAME##ClientMessagehandler4, MSG_LENGTH, ARG_TYPE1, ARG_NAME1,	\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4)				\

/**
	Client��Ϣ�꣬  ֻ�������������Ϣ
*/
#if defined(NETWORK_INTERFACE_DECLARE_BEGIN)
	#undef CLIENT_MESSAGE_HANDLER_ARGS5
#endif

#if defined(DEFINE_IN_INTERFACE)
#if defined(CLIENT)
#define CLIENT_MESSAGE_HANDLER_ARGS5(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5)				\
	void NAME##ClientMessagehandler5::handle(,		\
												MemoryStream& s)		\
	{																			\
			ARG_TYPE1 ARG_NAME1;												\
			s >> ARG_NAME1;														\
			ARG_TYPE2 ARG_NAME2;												\
			s >> ARG_NAME2;														\
			ARG_TYPE3 ARG_NAME3;												\
			s >> ARG_NAME3;														\
			ARG_TYPE4 ARG_NAME4;												\
			s >> ARG_NAME4;														\
			ARG_TYPE5 ARG_NAME5;												\
			s >> ARG_NAME5;														\
			KBEngineClient::CLIENTAPP::getInstance().NAME(					\
				ARG_NAME1, ARG_NAME2,										 	\
				ARG_NAME3, ARG_NAME4, ARG_NAME5);								\
	}																			\

#else
#define CLIENT_MESSAGE_HANDLER_ARGS5(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5)				\
	void NAME##ClientMessagehandler5::handle(,		\
												MemoryStream& s)		\
	{																			\
	}																			\
		
#endif
#else
#define CLIENT_MESSAGE_HANDLER_ARGS5(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5)				\
	class NAME##ClientMessagehandler5 : public Mercury::MessageHandler			\
	{																			\
	public:																		\
		virtual void handle(,							\
							MemoryStream& s);							\
	};																			\

#endif

#define CLIENT_MESSAGE_DECLARE_ARGS5(NAME, MSG_LENGTH, ARG_TYPE1, ARG_NAME1,	\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5)				\
	CLIENT_MESSAGE_HANDLER_ARGS5(NAME, ARG_TYPE1, ARG_NAME1, 					\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5)				\
	NETWORK_MESSAGE_DECLARE_ARGS5(Client, NAME,									\
				NAME##ClientMessagehandler5, MSG_LENGTH, ARG_TYPE1, ARG_NAME1,	\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5)				\


/**
	Client��Ϣ�꣬  ֻ��������������Ϣ
*/
#if defined(NETWORK_INTERFACE_DECLARE_BEGIN)
	#undef CLIENT_MESSAGE_HANDLER_ARGS6
#endif

#if defined(DEFINE_IN_INTERFACE)
#if defined(CLIENT)
#define CLIENT_MESSAGE_HANDLER_ARGS6(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5,				\
											ARG_TYPE6, ARG_NAME6)				\
	void NAME##ClientMessagehandler6::handle(,		\
												MemoryStream& s)		\
	{																			\
			ARG_TYPE1 ARG_NAME1;												\
			s >> ARG_NAME1;														\
			ARG_TYPE2 ARG_NAME2;												\
			s >> ARG_NAME2;														\
			ARG_TYPE3 ARG_NAME3;												\
			s >> ARG_NAME3;														\
			ARG_TYPE4 ARG_NAME4;												\
			s >> ARG_NAME4;														\
			ARG_TYPE5 ARG_NAME5;												\
			s >> ARG_NAME5;														\
			ARG_TYPE6 ARG_NAME6;												\
			s >> ARG_NAME6;														\
			KBEngineClient::CLIENTAPP::getInstance().NAME(					\
				ARG_NAME1, ARG_NAME2,										 	\
				ARG_NAME3, ARG_NAME4, ARG_NAME5, ARG_NAME6);					\
	}																			\

#else
#define CLIENT_MESSAGE_HANDLER_ARGS6(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5,				\
											ARG_TYPE6, ARG_NAME6)				\
	void NAME##ClientMessagehandler6::handle(,		\
												MemoryStream& s)		\
	{																			\
	}																			\
		
#endif
#else
#define CLIENT_MESSAGE_HANDLER_ARGS6(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5,				\
											ARG_TYPE6, ARG_NAME6)				\
	class NAME##ClientMessagehandler6 : public Mercury::MessageHandler			\
	{																			\
	public:																		\
		virtual void handle(,							\
							MemoryStream& s);							\
	};																			\

#endif

#define CLIENT_MESSAGE_DECLARE_ARGS6(NAME, MSG_LENGTH, ARG_TYPE1, ARG_NAME1,	\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5,				\
											ARG_TYPE6, ARG_NAME6)				\
	CLIENT_MESSAGE_HANDLER_ARGS6(NAME, ARG_TYPE1, ARG_NAME1, 					\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5,				\
											ARG_TYPE6, ARG_NAME6)				\
	NETWORK_MESSAGE_DECLARE_ARGS6(Client, NAME,									\
				NAME##ClientMessagehandler6, MSG_LENGTH, ARG_TYPE1, ARG_NAME1,	\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5,				\
											ARG_TYPE6, ARG_NAME6)				\

/**
	Client��Ϣ�꣬  ֻ�а˸���������Ϣ
*/
#if defined(NETWORK_INTERFACE_DECLARE_BEGIN)
	#undef CLIENT_MESSAGE_HANDLER_ARGS8
#endif

#if defined(DEFINE_IN_INTERFACE)
#if defined(CLIENT)
#define CLIENT_MESSAGE_HANDLER_ARGS8(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5,				\
											ARG_TYPE6, ARG_NAME6,				\
											ARG_TYPE7, ARG_NAME7,				\
											ARG_TYPE8, ARG_NAME8)				\
	void NAME##ClientMessagehandler8::handle(,		\
												MemoryStream& s)		\
	{																			\
			ARG_TYPE1 ARG_NAME1;												\
			s >> ARG_NAME1;														\
			ARG_TYPE2 ARG_NAME2;												\
			s >> ARG_NAME2;														\
			ARG_TYPE3 ARG_NAME3;												\
			s >> ARG_NAME3;														\
			ARG_TYPE4 ARG_NAME4;												\
			s >> ARG_NAME4;														\
			ARG_TYPE5 ARG_NAME5;												\
			s >> ARG_NAME5;														\
			ARG_TYPE6 ARG_NAME6;												\
			s >> ARG_NAME6;														\
			ARG_TYPE7 ARG_NAME7;												\
			s >> ARG_NAME7;														\
			ARG_TYPE8 ARG_NAME8;												\
			s >> ARG_NAME8;														\
			KBEngineClient::CLIENTAPP::getInstance().NAME(					\
										ARG_NAME1, ARG_NAME2, ARG_NAME3, 		\
										ARG_NAME4, ARG_NAME5, ARG_NAME6,		\
										ARG_NAME7, ARG_NAME8);					\
	}																			\

#else
#define CLIENT_MESSAGE_HANDLER_ARGS8(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5,				\
											ARG_TYPE6, ARG_NAME6,				\
											ARG_TYPE7, ARG_NAME7,				\
											ARG_TYPE8, ARG_NAME8)				\
	void NAME##ClientMessagehandler8::handle(,		\
												MemoryStream& s)		\
	{																			\
	}																			\
		
#endif
#else
#define CLIENT_MESSAGE_HANDLER_ARGS8(NAME, ARG_TYPE1, ARG_NAME1,				\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5,				\
											ARG_TYPE6, ARG_NAME6,				\
											ARG_TYPE7, ARG_NAME7,				\
											ARG_TYPE8, ARG_NAME8)				\
	class NAME##ClientMessagehandler8 : public Mercury::MessageHandler			\
	{																			\
	public:																		\
		virtual void handle(,							\
							MemoryStream& s);							\
	};																			\

#endif

#define CLIENT_MESSAGE_DECLARE_ARGS8(NAME, MSG_LENGTH, ARG_TYPE1, ARG_NAME1,	\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5,				\
											ARG_TYPE6, ARG_NAME6,				\
											ARG_TYPE7, ARG_NAME7,				\
											ARG_TYPE8, ARG_NAME8)				\
	CLIENT_MESSAGE_HANDLER_ARGS8(NAME, ARG_TYPE1, ARG_NAME1, 					\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5,				\
											ARG_TYPE6, ARG_NAME6,				\
											ARG_TYPE7, ARG_NAME7,				\
											ARG_TYPE8, ARG_NAME8)				\
	NETWORK_MESSAGE_DECLARE_ARGS8(Client, NAME,									\
				NAME##ClientMessagehandler8, MSG_LENGTH, ARG_TYPE1, ARG_NAME1,	\
											ARG_TYPE2, ARG_NAME2,				\
											ARG_TYPE3, ARG_NAME3,				\
											ARG_TYPE4, ARG_NAME4,				\
											ARG_TYPE5, ARG_NAME5,				\
											ARG_TYPE6, ARG_NAME6,				\
											ARG_TYPE7, ARG_NAME7,				\
											ARG_TYPE8, ARG_NAME8)				\


}
#endif