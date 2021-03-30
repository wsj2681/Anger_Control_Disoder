#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "framework.h"

#define MAXTHREAD 5


// 상하좌우 상수값 설정
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77



CRITICAL_SECTION cs;
// ThreadFunction
DWORD WINAPI PlayerThread(LPVOID arg);

// Thread ID
DWORD dwThreadID[MAXTHREAD];

//스레드 갯수
static int idIndex = 1;

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

	InitializeCriticalSection(&cs);

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


	DeleteCriticalSection(&cs);
	closesocket(listen_sock);

	WSACleanup();

	return 0;
}

DWORD WINAPI PlayerThread(LPVOID arg)
{
	int retval;
	Thread_id thread_id;
	thread_id.thread_num = idIndex;
	idIndex++;

	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN client_addr;
	int client_addr_len;
	char buf[2];
	char GameReady[6];
	ZeroMemory(&buf, sizeof(buf));
	
	Player_world player;

	Player_world other_player;
	
	char keyboard;
	
	////쓰레드 카운터
	//++thread_count;



	getpeername(client_sock, (SOCKADDR*)&client_addr, &client_addr_len);


	////송수신/////////////////////////////////////
	retval = recv(client_sock, (char*)GameReady, sizeof(GameReady), 0);
	cout << GameReady << "받기 완료" << endl;

	char GameOk[7] = "GameOk";
	retval = send(client_sock, (char*)GameOk, sizeof(GameOk), 0);
	cout << GameOk << "전송 완료" << endl;

	//id.thread_id =thread_count;

	
	cout << "lll thread_id = " << thread_id.thread_num << endl;
	retval = send(client_sock, (char*)&thread_id, sizeof(thread_id), 0);
	cout << "llll  thread_id = " << thread_id.thread_num << endl;
	///////////////////////////

	

	while (true) {

		//스레드 아이디 초기화
		thread_id.thread_num = 0;
		
		retval = recv(client_sock, (char*)&thread_id, sizeof(thread_id), 0);
		
		
		
		
		EnterCriticalSection(&cs);
		if (thread_id.thread_num == 1) {
			retval = recv(client_sock, (char*)&player, sizeof(player), 0);
			//cout << player.player_world._41 << " ," << player.player_world._42 << ", " << player.player_world._43 << endl;
			retval = send(client_sock, (char*)&other_player, sizeof(other_player), 0);
			cout << other_player.player_world._41 << ", " << other_player.player_world._42 << ", " << other_player.player_world._43 << endl;

		}
		else if (thread_id.thread_num == 2) {
			
			retval = recv(client_sock, (char*)&other_player, sizeof(other_player), 0);
			//cout << other_player.player_world._41 << ", " << other_player.player_world._42 << ", " << other_player.player_world._43 << endl;

			retval = send(client_sock, (char*)&player, sizeof(player), 0);
			cout << player.player_world._41 << " ," << player.player_world._42 << ", " << player.player_world._43 << endl;

		}

		LeaveCriticalSection(&cs);
		

		//////월드좌표계 초기화
		//XMStoreFloat4x4(&other_player.player_world, XMMatrixIdentity());

		////서버에서 직접 키보드 입력 받기 
		//if (_kbhit()) {        //키보드 입력 확인 (true / false)
		//	keyboard = _getch();      // 방향키 입력시 224 00이 들어오게 되기에 앞에 있는 값 224를 없앰
		//	if (keyboard == -32) {    // -32로 입력되면
		//		keyboard = _getch();  // 새로 입력값을 판별하여 상하좌우 출력
		//		switch (keyboard) {
		//		case LEFT:
		//			other_player.player_world._41 -= 1.0f;
		//			printf("왼쪽으로 이동\n");
		//			break;
		//		case RIGHT:
		//			other_player.player_world._41 += 1.0f;
		//			printf("오른쪽으로 이동\n");
		//			break;
		//		case UP:
		//			other_player.player_world._43 += 1.0f;
		//			printf("위로 이동\n");
		//			break;
		//		case DOWN:
		//			other_player.player_world._43 -= 1.0f;
		//			printf("아래로 이동\n");
		//			break;
		//		}
		//	}
		//}

		////값보내기
		//retval = send(client_sock, (char*)&other_player, sizeof(other_player), 0);
		//cout << other_player.player_world._41 << " " << other_player.player_world._42 << " " << other_player.player_world._43 << endl;
		//
		


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
