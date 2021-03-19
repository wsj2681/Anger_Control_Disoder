#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include "stdafx.h"
#include "Sever.h"

Sever::Sever()
{
	
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		cout << "WSAStartup Error" << endl;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN sever_sock_addr;

	::ZeroMemory(&sever_sock_addr, sizeof(SOCKADDR_IN));
	sever_sock_addr.sin_family = AF_INET;
	sever_sock_addr.sin_addr.S_un.S_addr = inet_addr(SERVERIP);
	sever_sock_addr.sin_port = ntohs(SERVERPORT);


	retval = connect(sock, (sockaddr*)&sever_sock_addr, sizeof(sever_sock_addr));


}

void Sever::Sever_send()
{
	//준비완료 보내기
	retval = send(sock, (char*)"Ready", sizeof("Ready"), 0);
	cout << "보내기완료" << endl;
	
}

void Sever::Sever_recv()
{
	//준비완료 받기
	retval = recv(sock, (char*)Save_Data, sizeof BUFSIZE, 0);
	cout << Save_Data <<"받기완료" << endl;

}

