// Progetto_PDS_1.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include <thread>
#include "Progetto_PDS_1.h"
#include <oleacc.h>
#include <cstdio>

MyProcessManager& p = MyProcessManager::CreateInstance();
int c=0;
BOOL CALLBACK EnumWindowsPrc(HWND hwnd, LPARAM lParam)
{
	char szProcessName[MAX_PATH] = "";
	// Get a handle to the process.
	HMODULE hMod;
	DWORD cbNeeded;
	GetWindowThreadProcessId(hwnd, &cbNeeded);
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, cbNeeded);
	// Get the process name.
	if (hProcess != NULL)
	{
		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
			&cbNeeded))
		{
			GetModuleBaseNameA(hProcess, hMod, szProcessName,
				sizeof(szProcessName) / sizeof(TCHAR));
		}
	}
	std::string s(szProcessName);
	CloseHandle(hProcess);
	// Print the process name and identifier.
	if (s.length() == 0) return true;
	GetClassNameA(hwnd, szProcessName, 1024);
	if (IsWindowEnabled(hwnd) && IsWindowVisible(hwnd) && strlen(szProcessName)!=0)
		c++;
	return true;
}
void CALLBACK WinEventProcFC(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {
	DWORD c;
	GetWindowThreadProcessId(hwnd, &c);
	std::cout << c << std::endl;
	p.UpdateFocus(c);
}

void CALLBACK HandleWinEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
	{

		
			IAccessible* pAcc = NULL;
			VARIANT varChild;

			HRESULT hr = AccessibleObjectFromEvent(hwnd, idObject, idChild, &pAcc, &varChild);

			if ((hr == S_OK) && (pAcc != NULL))
			{
				VARIANT varResult = {};
				pAcc->get_accRole(varChild, &varResult);

				if (varResult.lVal == ROLE_SYSTEM_PAGETAB || varResult.lVal == NULL)
				{
					BSTR bstrName;
					pAcc->get_accName(varChild, &bstrName);

					if (event == EVENT_OBJECT_CREATE)
					{
						printf("Create: ");
						p.AddElement();
					}
					else if (event == EVENT_OBJECT_DESTROY)
					{
						printf("Destroy:   ");
						p.RemoveElement();
					}
					printf("%S\n", bstrName);
					SysFreeString(bstrName);
				}
				pAcc->Release();
			}
		}



int main()
{


	
	p.InitClass();
	std::string str;

	//p.UpdateFocus();
	//Sleep(5000);
	//p.UpdateListProcess();
	//p.UpdateFocus();
	//std::cout << "fine" << std::endl;
	//Sleep(5000);
	//HOOKPROC* proc = proc1;

 
 std::thread t ([&]() { 
	MSG msg;

HWINEVENTHOOK hWinEventHook0 = SetWinEventHook(
		 EVENT_OBJECT_DESTROY, EVENT_OBJECT_DESTROY,
			 NULL, HandleWinEvent, 0, 0,
		 WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

		HWINEVENTHOOK hWinEventHook1 = SetWinEventHook(
		 EVENT_OBJECT_CREATE, EVENT_OBJECT_CREATE,
		 NULL, HandleWinEvent, 0, 0,
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
		
	 HWINEVENTHOOK h = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, 0, WinEventProcFC, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
	
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


			}
		);
	//SetWindowsHook(WH_CBT,);
// UnhookWinEvent(h);
	std::string st = p.PrintAll();
	std::cout << st<<std::endl<<st.length();
	My_Socket s;
	s.NewConnection_Server(1552);
	s.Sendind_Server((void*)st.c_str(),st.length());
	//UnhookWinEvent(h);
	Sleep(20000);

    return 0;
}


