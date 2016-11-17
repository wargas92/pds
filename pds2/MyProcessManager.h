#pragma once
#include <WinUser.h>
#include <Windows.h>
#include <string>
#include <Psapi.h>
#include "MyProcess.h"
#include <map>
#include <WinDef.h>
#include <mutex>
#include <olectl.h>
#include <string.h>

#pragma comment(lib, "oleaut32.lib")

class MyProcessManager
{
	static std::map < DWORD, MyProcess* > MyProcessManager::pMap;
	MyProcess* pFocus;
	std::mutex m;
	//BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
	MyProcessManager(const MyProcessManager&);
	//\MyProcessManager& operator= (const MyProcessManager&);
	MyProcessManager();


public:
	static int MyProcessManager::pcount;
	static MyProcessManager& CreateInstance();
	static MyProcess& getProcessByPid(DWORD pid);
	bool InitClass();
	~MyProcessManager();
	static bool getProcessByPid(DWORD, MyProcess*);
	//void UpdateListProcess();

	void UpdateFocus(DWORD pid);
	//void PrintAll();
	std::string PrintAll();
	void RemoveElement(DWORD);
	void RemoveElement();

	static void AddElement(MyProcess*);
	int WindowsElementCount();
	void MyProcessManager::AddElement();
};