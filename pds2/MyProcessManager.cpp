#include "stdafx.h"
#include "MyProcessManager.h"
#include <iostream>
#include <string.h>
#include "Shlwapi.h"
#include <vector>
#include <sstream>
#define SERVER_PORT 1500
#define MAX_LENGTH 1024

std::map  < DWORD, MyProcess* > MyProcessManager::pMap;
int MyProcessManager::pcount;
void PrintProcessNameAndID(DWORD);
void PrintProcessInfo(DWORD);
void PrintProcessInfo(MyProcess&);
HRESULT SaveIcon(HICON hIcon, const wchar_t* path);
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

MyProcessManager::MyProcessManager() { this->pFocus = new MyProcess(); }
MyProcessManager::~MyProcessManager() {}
bool MyProcessManager::getProcessByPid(DWORD pid, MyProcess * p)
{
	if (pMap[pid] == NULL) return false;
	p = pMap[pid];
	
	return true;
}
void MyProcessManager::UpdateFocus(DWORD pid)
{
	if (pMap.find(pid) == pMap.end()) return;
	MyProcess& p = MyProcessManager::getProcessByPid(pid);
	this->pFocus->setPid(pid);
	this->pFocus->setClassName(p.getClassName());
	this->pFocus->setMWND(p.getMWND());
	this->pFocus->setPNAME(p.getPName());
	this->pFocus->setTidWND(p.getTid());
	this->pFocus->setIsVisible(p.getIsVisible());
	this->pFocus->setTitleWND(p.getTitleWND());
	std::cout << "================== FOCUS PROCESS INFO ==================" << std::endl;
	PrintProcessInfo(*pFocus);
	std::cout << "================== END FOCUS PROCESS INFO ==================" << std::endl;

}



bool MyProcessManager::SetFocus(DWORD p)
{
	if (pMap[p] == NULL) return false;//this->AddElement();
	if (pMap[p]->getMWND() == NULL) return false;
	SetForegroundWindow(pMap[p]->getMWND());
	return true;
}


MyProcessManager& MyProcessManager::CreateInstance()
{
	static MyProcessManager mpm;
	return mpm;
}

MyProcess & MyProcessManager::getProcessByPid(DWORD pid)
{
	return *(MyProcessManager::pMap[pid]);
}

bool MyProcessManager::InitClass()
{
	this->pcount = 0;
	// Print the name and process identifier for each process.
	std::unique_lock<std::mutex> ul(this->m);
	
	this->sock.NewConnection_Server(SERVER_PORT);
	
	EnumWindows(EnumWindowsProc, NULL);

	/*std::thread t([&]() {
		
		this->s.NewConnection_Server(SERVER_PORT);
		std::thread t([&]() {
			char buffer[MAX_LENGTH + 1];
			int rec;
			while (1) {
				rec=this->s.Receiving_Server((void*)buffer, MAX_LENGTH);
				//Add code here to manage buffer received 
				//Easily client send first pid then number of commands then commands
				//			
			}
		});
		t.detach();
		std::string str = this->PrintAll().c_str();
		this->s.Sendind_Server((void*)str.c_str(),str.length());//Initial list
		//Add code here to manage destroy, focus, create;
		//Use cond_var, which is inside MyProcessManager. It is strongly recommended
	
	});*/
	//t.detach(); // see if a threadJoin is request
	std::string s(PrintAll());
	this->sock.Sendind_Server((void*)s.c_str(), s.length());
	return true;


}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{

	DWORD lpdwProcessId, tid;
	char buffer[1024] = "";

	DWORD d = GetCurrentProcessId();
	tid = GetWindowThreadProcessId(hwnd, &lpdwProcessId);

		if (IsWindowEnabled(hwnd) &&IsWindowVisible(hwnd) ) {
			MyProcess * c = new MyProcess(lpdwProcessId);
			c->setIsVisible(TRUE);
			c->setTidWND(tid);
			GetClassNameA(hwnd, buffer, sizeof(buffer));
			c->setMWND(hwnd);
			if(strlen(buffer))
			c->setClassName(buffer);
			if (d == lpdwProcessId)
				SetWindowTextA(hwnd, "Remote Controller-Server Side");
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, lpdwProcessId);
			// Get the process name.

			if (hProcess != NULL) {
				HICON h;
				GetProcessImageFileNameA(hProcess, buffer, sizeof(buffer));
				std::string s(PathFindFileNameA(buffer));
				c->setPNAME(s);
				GetModuleFileNameExA(hProcess, 0, buffer, sizeof(buffer));
				c->setIcon(buffer);
				h = c->getIcon();
				std::wstring st(s.begin(), s.end());
				st = L"c:\\test\\" + st + L".ico";
				const wchar_t * path = st.c_str();
				SaveIcon(h, st.c_str());
				CloseHandle(hProcess);
			}



			GetWindowTextA(hwnd, buffer, sizeof(buffer));
			if (strlen(buffer))
				c->setTitleWND(buffer);
			MyProcessManager::AddElement(c);
		}
	

	return TRUE;
}

void PrintProcessInfo(DWORD pid) {
	
	MyProcess p = MyProcessManager::getProcessByPid(pid);
	if (!p.getIsVisible()) return;
	std::cout << "Process id: " << p.getPid() << std::endl;
	std::cout << "Process name: " << p.getPName() << std::endl;
	std::cout << "Window title: " << p.getTitleWND() << std::endl;
	std::cout << "IsWindowVisible: " << p.getIsVisible() << std::endl;
	std::cout << "Class name: " << p.getClassName() << std::endl << std::endl;

}


void PrintProcessInfo(MyProcess& p) {


	if (!p.getIsVisible()) return;
	std::cout << "Process id: " << p.getPid() << std::endl;
	std::cout << "Process name: " << p.getPName() << std::endl;
	std::cout << "Window title: " << p.getTitleWND() << std::endl;
	std::cout << "IsWindowVisible: " << p.getIsVisible() << std::endl;
	std::cout << "Class name: " << p.getClassName() << std::endl << std::endl;




}


std::string MyProcessManager::PrintAll() {
	std::string s;

	for (auto i = pMap.begin(); i != pMap.end(); i++) {
		PrintProcessInfo(*(*i).second);
		if (i->second->getClassName().length() != 0)
			s += i->second->toString();
	}
	return s;
}

void MyProcessManager::RemoveElement(DWORD c)
{
	
	delete pMap[c];
	pMap.erase(c);
}

void MyProcessManager::RemoveElement()
{
	std::unique_lock<std::mutex> ul(this->m);
	std::ostringstream oss;
	
	for (auto i = pMap.begin(); i != pMap.end(); i++)
	{
		oss << i->second->getPid() << " ";
		delete i->second;
	}
	std::string str(oss.str());
	this->pMap.clear();
	EnumWindows(EnumWindowsProc, NULL);
	std::ostringstream oss1;
	for (auto i = pMap.begin(); i != pMap.end(); i++)
		oss1 << i->second->getPid() << " ";

	std::string str1(oss1.str());
	if (str.compare(str1) == 0) return;

	std::istringstream iss(str);

	std::string word,word1;
	
	while (iss >> word) {
		std::istringstream	iss1(str1);
		bool flag = true;
		while (iss1 >> word1)
			if (word1.compare(word) == 0) { flag = false; break; }

		if (flag) {
			std::cout << "Element removed with pid: " << word << std::endl;
		}
	}
}


void MyProcessManager::AddElement(MyProcess * c)
{
	if (c != NULL)
	{
		if (pMap[c->getPid()] == NULL) pMap[c->getPid()] = c;
		else { 
			
			pMap[c->getPid()]->setTitleWND(c->getTitleWND());
			pMap[c->getPid()]->setClassName(c->getClassName()) ;
			
			delete c; }
	}
}

int MyProcessManager::WindowsElementCount()
{
	std::unique_lock<std::mutex> ul(this->m);
	return this->pcount;
}

void MyProcessManager::AddElement()
{
	std::unique_lock<std::mutex> ul(this->m);

	std::ostringstream oss;
	for (auto i = pMap.begin(); i != pMap.end(); i++)
		oss<<i->second->getPid() << " ";
	std::string str (oss.str());
	//this->pMap.clear();
	EnumWindows(EnumWindowsProc, NULL);
	std::ostringstream oss1;
	for (auto i = pMap.begin(); i != pMap.end(); i++)
		oss1<<i->second->getPid() << " ";
	
	std::string str1(oss1.str());
	if (str.compare(str1) == 0) return;

	std::istringstream iss1(str1);
	std::string word, word1;

	while (iss1 >> word1) {
		std::istringstream	iss(str);
		bool flag = true;
		while (iss >> word)
			if (word1.compare(word) == 0) { flag = false; break; }
		if (flag) {
			PrintProcessInfo(*pMap[std::stoi(word1)]);
			std::string str (pMap[std::stoi(word1)]->toString());
			this->sock.Sendind_Server((void*)str.c_str(), str.length());
			std::cout << str << std::endl;
		}
			
	}

}


HRESULT SaveIcon(HICON hIcon, const wchar_t* path) {
	// Create the IPicture intrface
	PICTDESC desc = { sizeof(PICTDESC) };
	desc.picType = PICTYPE_ICON;
	desc.icon.hicon = hIcon;
	IPicture* pPicture = 0;
	HRESULT hr = OleCreatePictureIndirect(&desc, IID_IPicture, FALSE, (void**)&pPicture);
	if (FAILED(hr)) return hr;

	// Create a stream and save the image
	IStream* pStream = 0;
	CreateStreamOnHGlobal(0, TRUE, &pStream);
	LONG cbSize = 0;
	hr = pPicture->SaveAsFile(pStream, TRUE, &cbSize);

	// Write the stream content to the file
	if (!FAILED(hr)) {
		HGLOBAL hBuf = 0;
		GetHGlobalFromStream(pStream, &hBuf);
		void* buffer = GlobalLock(hBuf);
		HANDLE hFile = CreateFileW(path, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		if (!hFile) hr = HRESULT_FROM_WIN32(GetLastError());
		else {
			DWORD written = 0;
			WriteFile(hFile, buffer, cbSize, &written, 0);
			CloseHandle(hFile);
		}
		GlobalUnlock(buffer);
	}
	// Cleanup
	pStream->Release();
	pPicture->Release();
	return hr;
}
