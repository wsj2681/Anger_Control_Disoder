#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

using namespace std;



#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512


class Sever
{

public:

	SOCKET sock;
	int retval = 0;
	char Save_Data[BUFSIZE];

	Sever();
	~Sever();

	void Sever_send();

	void Sever_recv();
	
	

};

