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
	//�غ�Ϸ� ������
	retval = send(sock, (char*)"Ready", sizeof("Ready"), 0);
	cout << "������Ϸ�" << endl;
	
}

void Sever::Sever_recv()
{
	//�غ�Ϸ� �ޱ�
	retval = recv(sock, (char*)Save_Data, sizeof BUFSIZE, 0);
	cout << Save_Data <<"�ޱ�Ϸ�" << endl;

}

