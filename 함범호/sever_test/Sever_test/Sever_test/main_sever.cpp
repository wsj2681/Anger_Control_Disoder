#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "framework.h"

#define MAXTHREAD 5



CRITICAL_SECTION cs;
// ThreadFunction
DWORD WINAPI PlayerThread(LPVOID arg);

// Thread ID
DWORD dwThreadID[MAXTHREAD];

//스레드 갯수
static int idIndex = 1;

//HANDLE hThread[MAXTHREAD];
HANDLE hThread;
int threadCount = 0;

//공유자원
Player_world thread_num_1_player;
Player_world thread_num_2_player;
int thread_empty[MAXTHREAD] = { 0, };
bool using_recv = false;

struct SESSION {
	WSABUF dataBuffer;
	SOCKET socket;

};

map<SOCKET, SESSION>clients;

//void err_quit(const char* msg);
//void err_display(const char* msg);

int main()
{
	int retval = 0;

	wcout.imbue(locale("korean"));
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

		//hThread[threadCount] = CreateThread(nullptr, 0, PlayerThread, (LPVOID)client_sock, 0, &dwThreadID[threadCount]);
		//threadCount++;

		hThread = CreateThread(nullptr, 0, PlayerThread, (LPVOID)client_sock, 0, NULL);

		if (hThread == NULL)
			closesocket(client_sock);
		else { CloseHandle(hThread); }
	}

	/*for (auto& i : hThread)
		CloseHandle(i);*/


	DeleteCriticalSection(&cs);
	closesocket(listen_sock);

	WSACleanup();

	return 0;
}

DWORD WINAPI PlayerThread(LPVOID arg)
{
	int retval;
	Thread_id thread_id;
	/*thread_id.thread_num = idIndex;
	idIndex++;*/


	SOCKET thread_client_sock = (SOCKET)arg;
	SOCKADDR_IN client_addr;
	int thread_client_addr_len;
	char buf[2];
	char GameReady[6];
	ZeroMemory(&buf, sizeof(buf));


	//mian 으로 뺴는게 좋지않을까? 공유자원이라서 임계영역 설정시 프레임 드랍 예상됨
	for (int i = 0; i < MAXTHREAD; ++i) {
		if (thread_empty[i] == 0) {
			thread_empty[i] = 1;
			thread_id.thread_num = i;
				
		}
		else {

			//쓰레드 해당개수(5개)를 다 할당했을떄
			closesocket(thread_client_sock);
			return 0;
		}
	}

	
	
	Player_world player;


	getpeername(thread_client_sock, (SOCKADDR*)&client_addr, &thread_client_addr_len);


	////송수신/////////////////////////////////////
	retval = recv(thread_client_sock, (char*)GameReady, sizeof(GameReady), 0);
	cout << GameReady << "받기 완료" << endl;

	char GameOk[7] = "GameOk";
	retval = send(thread_client_sock, (char*)GameOk, sizeof(GameOk), 0);
	cout << GameOk << "전송 완료" << endl;

	//id.thread_id =thread_count;

	
	cout << "lll thread_id = " << thread_id.thread_num << endl;
	retval = send(thread_client_sock, (char*)&thread_id, sizeof(thread_id), 0);
	cout << "llll  thread_id = " << thread_id.thread_num << endl;
	///////////////////////////

	////월드좌표계 초기화
	//XMStoreFloat4x4(&other_player.player_world, XMMatrixIdentity());
	XMStoreFloat4x4(&player.player_world, XMMatrixIdentity());

	XMStoreFloat4x4(&thread_num_1_player.player_world, XMMatrixIdentity());
	XMStoreFloat4x4(&thread_num_2_player.player_world, XMMatrixIdentity());


	while (true) {

		//스레드 아이디 초기화
		thread_id.thread_num = 0;
		
		retval = recv(thread_client_sock, (char*)&thread_id, sizeof(thread_id), 0);
		if (retval == SOCKET_ERROR) {
			display_error("recv : ", WSAGetLastError());
			break;
		}
		else if (retval == 0)
			break;
		retval = recv(thread_client_sock, (char*)&player, sizeof(player), 0);
		if (retval == SOCKET_ERROR) {
			display_error("recv : ", WSAGetLastError());
			break;
		}
		else if (retval == 0)
			break;
		

		EnterCriticalSection(&cs);
		
		if (thread_empty[ thread_id.thread_num] == 1 && using_recv == false) {
		
			thread_num_1_player = player;
			retval = send(thread_client_sock, (char*)&thread_num_2_player, sizeof(thread_num_2_player), 0);
			cout << "thread_1 of thread_2 value = " << thread_id.thread_num << " / " << thread_num_2_player.player_world._41 << 
				" ," << thread_num_2_player.player_world._42 << ", " << thread_num_2_player.player_world._43 << endl;
		
			using_recv = true;

		}
		else if (thread_empty[thread_id.thread_num] == 1 || using_recv == true) {
			
			
			thread_num_2_player = player;
			retval = send(thread_client_sock, (char*)&thread_num_1_player, sizeof(thread_num_1_player), 0);
			cout << "thread_2 of thread_1 value = " << thread_id.thread_num << " / " << thread_num_1_player.player_world._41 << 
				" ," << thread_num_1_player.player_world._42 << ", " << thread_num_1_player.player_world._43 << endl;

		
		}

		LeaveCriticalSection(&cs);
		

	}
	using_recv = false;
	closesocket(thread_client_sock);

	return 0;
}


void display_error(const char* msg, int err_no)
{
	WCHAR* lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	cout << msg;
	wcout << lpMsgBuf << endl;
	LocalFree(lpMsgBuf);
}

void err_quit(char* msg)
{

}

// 소켓 함수 오류 출력
void err_display(char* msg)
{

}
