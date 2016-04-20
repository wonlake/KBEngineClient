//
//  KNetworkInterface.cpp
//  libKBEClient
//
//  Created by Tom on 6/12/14.
//  Copyright (c) 2014 Tom. All rights reserved.
//

/*
	�ֽ��򽻻�����ģ�飺
		 ��������ͨ��һ�����BIG�ֽ���\Ҳ���������ֽ���.
 		 ����ʹ�õ�PC������Ƕ��ʽϵͳ����ʹ��BIG�ֽ���Ҳ����ʹ��LITTEN(С�ֽ���)
 		 �������Ǳ����ڴ�֮����һ���ֽ����ת����
*/
#ifndef __MEMORYSTREAMCONVERTER_H__
#define __MEMORYSTREAMCONVERTER_H__
// common include
#include <algorithm>
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#include <errno.h>
#endif
	
#include "basic_types.h"

namespace KBEngineClient{

/** ���������ֽ��� */
#define KBENGINE_LITTLE_ENDIAN							0
#define KBENGINE_BIG_ENDIAN								1
#if !defined(KBENGINE_ENDIAN)
#  if defined (ACE_BIG_ENDIAN)
#    define KBENGINE_ENDIAN KBENGINE_BIG_ENDIAN
#  else 
#    define KBENGINE_ENDIAN KBENGINE_LITTLE_ENDIAN
#  endif 
#endif

	namespace KMemoryStreamConverter
	{
		template<size_t T>
			inline void convert(char *val)
		{
			std::swap(*val, *(val + T - 1));
			convert<T - 2>(val + 1);
		}

		template<> inline void convert<0>(char *) {}
		template<> inline void convert<1>(char *) {}            // ignore central byte

		template<typename T> inline void apply(T *val)
		{
			convert<sizeof(T)>((char *)(val));
		}

		inline void convert(char *val, size_t size)
		{
			if(size < 2)
				return;

			std::swap(*val, *(val + size - 1));
			convert(val + 1, size - 2);
		}
	}

#if KBENGINE_ENDIAN == KBENGINE_BIG_ENDIAN			// ����ʹ��sys.isPlatformLittleEndian() ���в���
	template<typename T> inline void EndianConvert(T& val) { KMemoryStreamConverter::apply<T>(&val); }
	template<typename T> inline void EndianConvertReverse(T&) { }
#else
	template<typename T> inline void EndianConvert(T&) { }
	template<typename T> inline void EndianConvertReverse(T& val) { KMemoryStreamConverter::apply<T>(&val); }
#endif

	template<typename T> void EndianConvert(T*);         // will generate link error
	template<typename T> void EndianConvertReverse(T*);  // will generate link error

	inline void EndianConvert(uint8&) { }
	inline void EndianConvert(int8&) { }
	inline void EndianConvertReverse(uint8&) { }
	inline void EndianConvertReverse(int8&) { }

}
#endif