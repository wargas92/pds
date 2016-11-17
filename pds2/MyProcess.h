#pragma once
#include "stdafx.h"
#include <WinUser.h>
#include <Windows.h>
#include <string>
#include <Psapi.h>
#include <WinDef.h>

#define MAXL 1023

class MyProcess
{
	DWORD pid, tidWND;
	HWND mWND;
	std::string className, titleWND, pName;
	bool isVisible;

public:
	MyProcess(DWORD pid);
	MyProcess();
	MyProcess(const MyProcess&);
	MyProcess& operator= (const MyProcess&);
	~MyProcess();
	bool setTidWND(DWORD);
	bool setPid(DWORD);
	bool setMWND(HWND);
	bool setClassName(std::string);
	bool setTitleWND(std::string);
	bool setPNAME(std::string);
	DWORD getPid();
	HWND getMWND();
	DWORD getTid();
	std::string getClassName();
	std::string getPName();
	std::string getTitleWND();
	bool getIsVisible();
	void setIsVisible(bool);
	std::string toString();



};

