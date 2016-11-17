#include "stdafx.h"
#include "My_Socket.h"
#include <iostream>
#include<WinSock2.h>
#include <Ws2tcpip.h>


My_Socket::My_Socket()
{
	
}


My_Socket::~My_Socket()
{
	closesocket(this->s);
}



int My_Socket::Binding(int port) {
	this->service.sin_family = AF_INET;

	//inet_pton(AF_INET,ip.c_str(), &(this->service.sin_addr.s_addr));
	this->service.sin_addr.s_addr = INADDR_ANY;
	this->service.sin_port = htons(port);

	int iResult = bind(this->s, (sockaddr *)&(this->service), sizeof(service));
	if (iResult == SOCKET_ERROR) {
		std::cout << "errore bind" << (WSAGetLastError()) << std::endl;
		return -1;
	}
	return 0;

}




int My_Socket::NewConnection_Client(int port,std::string ip){

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"Error at WSAStartup()\n");
		return -1;
	}
	this->s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//\this->Binding(port, ip);
	this->service.sin_family = AF_INET;

	inet_pton(AF_INET, (ip.c_str()), &(this->service.sin_addr));
	this->service.sin_port = htons(port);

	 iResult = connect(this->s, (SOCKADDR *)&(this->service), sizeof(service));
	if (iResult == SOCKET_ERROR) {
		std::cout << "errore connessione" << (WSAGetLastError()) << std::endl;
		return -1;
	}
	return 0;
}


int My_Socket::Receiving(void* buffer, int len)
{


	int rec = recv(this->s, (char*)buffer, len, 0);
	if (rec == 0) std::cout << "connessione chiusa " << std::endl;
	else if (rec > 0)  std::cout << rec << " bytes ricevuti" << std::endl;
	else  std::cout << "recv failed: " << WSAGetLastError() << std::endl;
	return rec;


}

int My_Socket::Receiving_Server(void* buffer, int len)
{


	int rec = recv(this->acc_s, (char*)buffer,len, 0);
	if (rec == 0) std::cout << "connessione chiusa " << std::endl;
	else if(rec > 0)  std::cout << rec<<" bytes ricevuti" << std::endl; 
	else  std::cout<< "recv failed: "<< WSAGetLastError()<<std::endl;
	return rec;


}

int My_Socket::Sendind(void* buffer, int len)
{
	int rec = send(this->s, (char*)buffer, len, 0);
	if (rec == SOCKET_ERROR) { std::cout << "errore send " << WSAGetLastError() <<std::endl; return -1; }
	return rec;
}
int My_Socket::Sendind_Server(void* buffer, int len)
{
	int rec = send(this->acc_s, (char*)buffer, len, 0);
	if (rec == SOCKET_ERROR) { std::cout << "errore send " << WSAGetLastError() << std::endl; return -1; }
	return rec;
}



int My_Socket::NewConnection_Server(int port){
	sockaddr_in client_connect;
	char p[20];

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"Error at WSAStartup()\n");
		return -1;
	}
	this->s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);



	this->s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if((this->Binding(port))!=0) return -1;
	if (listen(this->s, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "listen function failed with error: " << WSAGetLastError() << std::endl;
		return -1;
	}
	int len= sizeof(client_connect);
	this->acc_s = accept(this->s, (sockaddr*)&(client_connect),&len);
	if (this->acc_s == INVALID_SOCKET) {
		std::cout<<"accept failed with error:"<< WSAGetLastError()<<std::endl;
		return -1;
	}
	else
	{
		inet_ntop(AF_INET,(void*)&(client_connect.sin_addr.s_addr),p,20);
	
		std::cout << "Client connesso con indirizzo " << p <<std::endl ;

	}return 0;
}




