// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
#include <string>
#include <stdio.h>
#include <locale>
#include <codecvt>

class StringEncodingUtility
{
public:
	StringEncodingUtility();
	~StringEncodingUtility();

public:
	static const std::string ws2s(const std::wstring& src);

	static const std::wstring s2ws(const std::string& src);

	static const std::string ws2utf8(const std::wstring& src);

	static const std::wstring utf8_2_ws(const std::string& src);
};
