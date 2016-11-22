// Progetto_PDS_1.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include <thread>
#include "Progetto_PDS_1.h"
#include <oleacc.h>
#include <cstdio>

MyProcessManager& p = MyProcessManager::CreateInstance();
int c=0;

void CALLBACK WinEventProcFC(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {
	DWORD c;
	GetWindowThreadProcessId(hwnd, &c);
	std::cout << c << std::endl;
	p.UpdateFocus(c);
}

void CALLBACK HandleWinEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
	{

		
			IAccessible* pAcc = NULL,*pAcc1=NULL;
			VARIANT varChild;
			IDispatch* pacc1;
			HRESULT hr = AccessibleObjectFromEvent(hwnd, idObject, idChild, &pAcc, &varChild);

			if ((hr == S_OK) && (pAcc != NULL))
			{
				VARIANT varResult = {};
				
				hr=pAcc->get_accChild(varChild,&pacc1);
				pAcc1 = (hr == S_OK && pacc1 != NULL) ? (IAccessible*)pacc1 : pAcc;
				pAcc1->get_accRole(varChild, &varResult);
				//LONG c = (hr == S_OK &&pacc1 != NULL) ? ROLE_SYSTEM_WINDOW : ROLE_SYSTEM_APPLICATION;
				if (varResult.lVal == ROLE_SYSTEM_APPLICATION || varResult.lVal == NULL)
				{
					BSTR bstrName;
					pAcc1->get_accName(varChild, &bstrName);

					if (event == EVENT_OBJECT_CREATE)
					{
						p.AddElement();
					}
					else if (event == EVENT_OBJECT_DESTROY)
					{
						p.RemoveElement();
					}
				}
				pAcc->Release();
			}
		}



int main()
{

	p.InitClass();
	std::string str;
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
	UnhookWinEvent(h);
	UnhookWinEvent(hWinEventHook1);
	UnhookWinEvent(hWinEventHook0);

			}
		);

	std::string st = p.PrintAll();
	std::cout << st <<std::endl<<st.length()<<std::endl;
	My_Socket s;
	s.NewConnection_Server(1552);
	//s.Sendind_Server((void*)st.c_str(),st.length());
	//Sleep(20000);

    return 0;
}


