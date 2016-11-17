#include "stdafx.h"

#include "MyProcess.h"





MyProcess::MyProcess(DWORD pid) :pid(pid)
{
}

MyProcess::MyProcess()
{
}

MyProcess::MyProcess(const MyProcess & c)
{
	this->className = c.className;
	this->mWND = c.mWND;
	this->pid = c.pid;
	this->pName = c.pName;
	this->tidWND = c.tidWND;
	this->titleWND = c.titleWND;
}


MyProcess & MyProcess::operator=(const MyProcess & c)
{
	MyProcess* c1 = new MyProcess(c.pid);
	c1->className = c.className;
	c1->mWND = c.mWND;
	//c1.pid = c.pid;
	c1->pName = c.pName;
	c1->tidWND = c.tidWND;
	c1->titleWND = c.titleWND;
	return *c1;

}

MyProcess::~MyProcess()
{


}

bool MyProcess::setTidWND(DWORD t)
{

	this->tidWND = t;

	return this->tidWND > 0;
}

bool MyProcess::setPid(DWORD p)
{
	this->pid = p;
	return true;
}

bool MyProcess::setMWND(HWND m)
{

	this->mWND = m;

	return this->mWND > 0;
}

bool MyProcess::setClassName(std::string cn)
{
	this->className = cn;

	return this->className.length() != 0;
}

bool MyProcess::setTitleWND(std::string cn)
{
	this->titleWND = cn;

	return this->titleWND.length() != 0;
}

bool MyProcess::setPNAME(std::string cn)
{
	this->pName = cn;

	return this->pName.length() != 0;
}

DWORD MyProcess::getPid()
{
	return this->pid;
}
DWORD MyProcess::getTid()
{
	return this->tidWND;
}
std::string MyProcess::getClassName()
{
	return className.length()!=0 ? this->className : "";
}
std::string MyProcess::getPName()
{
	return this->pName;
}
std::string MyProcess::getTitleWND()
{
	return this->titleWND;
}

bool MyProcess::getIsVisible()
{
	return this->isVisible;
}

void MyProcess::setIsVisible(bool c)
{
	this->isVisible = c;
}

std::string MyProcess::toString()
{
	std::stringstream str;
	str << std::to_string(this->getPid()) << "," << this->getPName() << "," << this->getClassName() << "," << this->getTitleWND() << std::endl;
	
	return str.str();
}

HWND MyProcess::getMWND()
{
	return this->mWND;
}

DWORD MyProcess::setIcon(char * cn)
{
	SHFILEINFOA sfi = { 0 };
	bool res;

	if(res = SHGetFileInfo(cn, NULL, &sfi, sizeof(sfi), SHGFI_ICON))
		this->icon = sfi.hIcon;
	return res;
}

HICON MyProcess::getIcon()
{
	return this->icon;
}