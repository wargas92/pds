#pragma once
#include <WinSock2.h>
#include <iostream>
class My_Socket
{
	SOCKET s, acc_s;
	sockaddr_in service;
	int Binding(int port);
public:
	My_Socket();
	~My_Socket();
	int NewConnection_Server(int port);
	int NewConnection_Client(int port, std::string ip);
	int Receiving(void* buffer, int len);
	int Receiving_Server(void* buffer, int len);
	int Sendind(void* buffer, int len);
	int Sendind_Server(void* buffer, int len);
	
};

