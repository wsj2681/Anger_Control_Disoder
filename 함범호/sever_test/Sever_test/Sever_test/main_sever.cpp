#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "framework.h"

#define MAXTHREAD 5







// ThreadFunction
DWORD WINAPI PlayerThread(LPVOID arg);

// Thread ID
DWORD dwThreadID[MAXTHREAD];
static int idIndex = 0;
HANDLE hThread[MAXTHREAD];
int threadCount = 0;




//void err_quit(const char* msg);
//void err_display(const char* msg);

int main()
{
	int retval = 0;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	/*if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");*/

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	/*if (retval == SOCKET_ERROR)
		err_quit("bind()");*/

	retval = listen(listen_sock, SOMAXCONN);
	/*if (retval == SOCKET_ERROR)
		err_quit("listen()");*/

	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	int client_addr_len;
	char buf[BUFFERSIZE + 1];

	cout << "**** 서버 시작 ****" << endl;

	while (true)
	{
		client_addr_len = sizeof(client_addr);
		client_sock = accept(listen_sock, (SOCKADDR*)&client_addr, &client_addr_len);

	

		cout <<
			"\n[TCP 서버] 클라이언트 접속: IP 주소= " << inet_ntoa(client_addr.sin_addr) <<
			", 포트 번호=" << ntohs(client_addr.sin_port) << endl;

		hThread[threadCount] = CreateThread(nullptr, 0, PlayerThread, (LPVOID)client_sock, 0, &dwThreadID[threadCount]);

		threadCount++;
	}

	for (auto& i : hThread)
		CloseHandle(i);



	closesocket(listen_sock);

	WSACleanup();

	return 0;
}

DWORD WINAPI PlayerThread(LPVOID arg)
{
	int retval;
	int threadIndex = idIndex;
	idIndex++;
	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN client_addr;
	int client_addr_len;
	char buf[2];
	char GameReady[6];
	ZeroMemory(&buf, sizeof(buf));
	
	Player_world player;
	
	
	
	
	
	getpeername(client_sock, (SOCKADDR*)&client_addr, &client_addr_len);




	////송수신/////////////////////////////////////
	retval = recv(client_sock, (char*)GameReady, sizeof(GameReady), 0);
	cout << GameReady << "받기 완료" << endl;



	char GameOk[7] = "GameOk";

	retval = send(client_sock, (char*)GameOk, sizeof(GameOk), 0);
	///////////////////////////

	while (true) {
		retval = recv(client_sock, (char*)&player, sizeof(player), 0);
		cout << player.player_world._41 << " " << player.player_world._42 << " " << player.player_world._43 << endl;

	}


	return 0;
}




void err_quit(char* msg)
{

}

// 소켓 함수 오류 출력
void err_display(char* msg)
{

}
