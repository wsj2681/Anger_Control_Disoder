#pragma once

#define WIN32_LEAN_AND_MEAN             
// Windows ��� ����
#include <windows.h>
// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>

#include <iostream>

using namespace std;

#include <winsock2.h>
#pragma comment(lib, "ws2_32")

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512

SOCKET sock;


class ClientSever
{

public:
	void err_quit(const char* msg);

	void err_display(const char* msg);
	

};

