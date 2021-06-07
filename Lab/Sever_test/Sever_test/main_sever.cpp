#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "framework.h"

#define MAXTHREAD 5


//임계영역설정
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

Player_world thread_num_1_player;
Player_world thread_num_2_player;


int thread_empty[MAXTHREAD] = { 0, };

void setPosition(XMFLOAT3& fl3x3, XMFLOAT4X4& fl4x4);

//애니메이션
AttackAndDefend recv_attackAnddefend_1;
AttackAndDefend recv_attackAnddefend_2;
bool checkAnimation(AttackAndDefend attAdef);

//충돌처리
BoundingOrientedBox player_obb[3];
collide col1;
collide col2;
void SetOBB(BoundingOrientedBox* obb, Thread_id id, const XMFLOAT3& center, const XMFLOAT3& extents, const XMFLOAT4& orientation);
bool checkcollition(BoundingOrientedBox& first_obb, BoundingOrientedBox& second_obb, int i);

HeadHitted thread_1_headHitted;
HeadHitted thread_2_headHitted;

BoundingOrientedBox lHand_obb[3];
BoundingOrientedBox rHand_obb[3];
BoundingOrientedBox rFoot_obb[3];
BoundingOrientedBox lFoot_obb[3];
BoundingOrientedBox Head_obb[3];
BoundingOrientedBox Spine_obb[3];

XMFLOAT3 saveColPostion[3];

//HP
PlayerHP thread_num_1_HP;
PlayerHP thread_num_2_HP;


int cou = 0;

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

	//vector<thread> t1;

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

		//thread t1{ PlayerThread };

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
	thread_id.thread_num = idIndex;
	idIndex++;

	//for (int i = 0; i < MAXTHREAD; ++i) {
	//	if (thread_empty[i] == 0) {
	//		thread_empty[i] = 1;
	//		thread_id.thread_num = i;
	//			
	//	}
	//	else {

	//		//쓰레드 해당개수(5개)를 다 할당했을떄
	//	}
	//}

	SOCKET thread_client_sock = (SOCKET)arg;
	SOCKADDR_IN client_addr;
	int thread_client_addr_len;
	char buf[2];
	char GameReady[6];

	XMFLOAT3 player_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 player_rHand = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 player_lHand = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 player_rFoot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 player_lFoot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 player_Head = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 player_Spine = XMFLOAT3(0.0f, 0.0f, 0.0f);

	bool colledFrist = false;

	ZeroMemory(&buf, sizeof(buf));

	Player_world player;
	AttackAndDefend attAdef;
	PlayerHP player_hp;


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
	XMStoreFloat4x4(&player.player_Head, XMMatrixIdentity());
	XMStoreFloat4x4(&player.player_lFoot, XMMatrixIdentity());
	XMStoreFloat4x4(&player.player_lHand, XMMatrixIdentity());
	XMStoreFloat4x4(&player.player_rFoot, XMMatrixIdentity());
	XMStoreFloat4x4(&player.player_rHand, XMMatrixIdentity());
	XMStoreFloat4x4(&player.player_Spine, XMMatrixIdentity());

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

		retval = recv(thread_client_sock, (char*)&attAdef, sizeof(attAdef), 0);
		if (retval == SOCKET_ERROR) {
			display_error("recv : ", WSAGetLastError());
			break;
		}
		else if (retval == 0)
			break;
		retval = recv(thread_client_sock, (char*)&player_hp, sizeof(player_hp), 0);
		if (retval == SOCKET_ERROR) {
			display_error("recv : ", WSAGetLastError());
			break;
		}
		else if (retval == 0)
			break;


		//충돌박스 만들기
		setPosition(player_position, player.player_world);

		SetOBB(player_obb, thread_id, player_position, XMFLOAT3(2.2f, 11.f, 2.2f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));

		setPosition(player_Head, player.player_Head);
		SetOBB(Head_obb, thread_id, player_Head, XMFLOAT3(0.8f, 1.5f, 0.8f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));

		setPosition(player_rHand, player.player_rHand);
		SetOBB(rHand_obb, thread_id, player_rHand, XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));

		setPosition(player_lHand, player.player_lHand);
		SetOBB(lHand_obb, thread_id, player_lHand, XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));

		setPosition(player_rFoot, player.player_rFoot);
		SetOBB(rFoot_obb, thread_id, player_rFoot, XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));

		setPosition(player_lFoot, player.player_lFoot);
		SetOBB(lFoot_obb, thread_id, player_lFoot, XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));

		setPosition(player_Spine, player.player_Spine);
		SetOBB(Spine_obb, thread_id, player_Spine, XMFLOAT3(1.f, 9.f, 1.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));


		//cout << thread_id.thread_num << "     " << player_position.x << endl;
		//cout << thread_id.thread_num << "    " << player_obb[thread_id.thread_num].Center.x << endl;
		/*if(cou % 4 == 0)
		cout << "SETOBB SUCCESS!" << endl;*/

		// 충돌된 주먹 값 저장.
		//saveColPostion[thread_id.thread_num] = player_rHand;

		EnterCriticalSection(&cs);

		if (thread_id.thread_num == 1 || thread_id.thread_num == 3) {

			thread_num_1_player = player;
			recv_attackAnddefend_1 = attAdef;
			thread_num_1_HP = player_hp;

			if (idIndex <= 2)
				thread_num_2_HP.playerHp = 100.0f;


			////첫번쨰접속 쓰레드한테 충돌처리
			//col1.check_collide = checkcollition(player_obb[0], player_obb[1], 0);

			//// 가드됫을경우
			//if (thread_num_2_player.nowState == STATE_GUARD_RIGHT_HEAD) {
			//	col1.rHand2rHand = checkcollition(rHand_obb[0], rHand_obb[1], 1);
			//	if (col1.rHand2rHand)
			//		col1.collidePosition = player_rHand;

			//	col1.lHand2rHand = checkcollition(lHand_obb[0], rHand_obb[1], 3);
			//	if (col1.lHand2rHand)
			//		col1.collidePosition = player_lHand;

			//	/*if (col1.rHand2rHand || col1.lHand2rHand)

			//		thread_2_headHitted = true;
			//	else
			//		col2.headHitted = true;*/
			//}
			//else if (thread_num_2_player.nowState == STATE_GUARD_LEFT_HEAD) {
			//	col1.rHand2lHand = checkcollition(rHand_obb[0], lHand_obb[1], 2);
			//	if (col1.rHand2lHand)
			//		col1.collidePosition = player_rHand;

			//	col1.lHand2lHand = checkcollition(lHand_obb[0], lHand_obb[1], 4);
			//	if (col1.lHand2lHand)
			//		col1.collidePosition = player_lHand;

			//	/*if (col1.rHand2lHand || col1.lHand2lHand)
			//		thread_2_headHitted = true;
			//	else
			//		col2.headHitted = true;*/
			//}
			//else {
			//	//공격 성공
			//		//오른손 공격
			//	col1.rHand2Head = checkcollition(rHand_obb[0], Head_obb[1], 5);
			//	if (col1.rHand2Head) {
			//		col1.collidePosition = player_rHand;
			//		thread_2_headHitted.leftHeadHitted = true;
			//		//player_hp[thread_id.thread_num].playerHp = player_hp[thread_id.thread_num].playerHp - 10.0f;
			//	}
			//	col1.rHand2Spine = checkcollition(rHand_obb[0], Spine_obb[1], 6);
			//	if (col1.rHand2Spine)
			//		col1.collidePosition = player_rHand;


			//	// if문을 바로 나가야하지않을까?
			//// 왼속공격
			//	col1.lHand2Head = checkcollition(lHand_obb[0], Head_obb[1], 7);
			//	if (col1.lHand2Head) {
			//		col1.collidePosition = player_lHand;
			//		thread_2_headHitted.rightHeadHitted = true;
			//		//player_hp[thread_id.thread_num].playerHp = player_hp[thread_id.thread_num].playerHp - 10.0f;
			//	}
			//	col1.lHand2Spine = checkcollition(lHand_obb[0], Spine_obb[1], 8);
			//	if (col1.lHand2Spine)
			//		col1.collidePosition = player_lHand;


			//}



			retval = send(thread_client_sock, (char*)&thread_num_2_player, sizeof(thread_num_2_player), 0);
			//retval = send(thread_client_sock, (char*)&col2, sizeof(col2), 0);
			/*cout << "thread_1 of thread_2 value = " << thread_id.thread_num << " / " << thread_num_2_player.player_world._41 <<
				" ," << thread_num_2_player.player_world._42 << ", " << thread_num_2_player.player_world._43 << endl;
		*/
		//cout << "collide _ position - " << col1.collidePosition.x << " " << col1.collidePosition.y << " " << col1.collidePosition.z << endl;
			
			retval = send(thread_client_sock, (char*)&recv_attackAnddefend_2, sizeof(recv_attackAnddefend_2), 0);
			
			//retval = send(thread_client_sock, (char*)&thread_2_headHitted, sizeof(thread_2_headHitted), 0);
			
			retval = send(thread_client_sock, (char*)&thread_num_2_HP, sizeof(thread_num_2_HP), 0);

			//충돌좌표 초기화
			col1.collidePosition = XMFLOAT3(0.0f, 0.0f, 0.0f);

			thread_2_headHitted.leftHeadHitted = false;
			thread_2_headHitted.rightHeadHitted = false;
			thread_2_headHitted.straightHtitted = false;

		}
		else if (thread_id.thread_num == 2 || thread_id.thread_num == 4) {


			thread_num_2_player = player;
			recv_attackAnddefend_2 = attAdef;
			thread_num_2_HP = player_hp;

			////두번쨰접속 쓰레드한테 충돌처리
			//col2.check_collide = checkcollition(player_obb[1], player_obb[0], 17);

			//// 가드됫을경우
			//if (thread_num_2_player.nowState == STATE_GUARD_RIGHT_HEAD) {
			//	col2.rHand2rHand = checkcollition(rHand_obb[1], rHand_obb[0], 9);
			//	if (col2.rHand2rHand)
			//		col2.collidePosition = player_rHand;

			//	col2.lHand2rHand = checkcollition(lHand_obb[1], rHand_obb[0], 11);
			//	if (col2.lHand2rHand)
			//		col2.collidePosition = player_lHand;

			//	/*if (col2.rHand2rHand || col2.lHand2rHand)
			//		thread_1_headHitted = false;
			//	else
			//		thread_1_headHitted  = true;*/
			//}
			//else if (thread_num_1_player.nowState == STATE_GUARD_LEFT_HEAD) {
			//	col2.rHand2lHand = checkcollition(rHand_obb[1], lHand_obb[0], 10);
			//	if (col2.rHand2lHand)
			//		col2.collidePosition = player_lHand;

			//	col2.lHand2lHand = checkcollition(lHand_obb[1], lHand_obb[0], 12);
			//	if (col2.lHand2lHand)
			//		col2.collidePosition = player_lHand;

			//	/*if (col2.rHand2lHand || col2.lHand2lHand)
			//		thread_1_headHitted = false;
			//	else
			//		thread_1_headHitted = true;*/
			//}
			//else {
			//	//공격 성공
			//		//오른손 공격
			//	col2.rHand2Head = checkcollition(rHand_obb[1], Head_obb[0], 13);
			//	if (col2.rHand2Head) {
			//		col2.collidePosition = player_rHand;
			//		thread_1_headHitted.leftHeadHitted = true;
			//		//player_hp[thread_id.thread_num].playerHp = player_hp[thread_id.thread_num].playerHp - 10.0f;
			//	}
			//	col2.rHand2Spine = checkcollition(rHand_obb[1], Spine_obb[0], 14);
			//	if (col2.rHand2Spine)
			//		col2.collidePosition = player_rHand;


			//	// 왼속공격
			//	col2.lHand2Head = checkcollition(lHand_obb[1], Head_obb[0], 15);
			//	if (col2.lHand2Head) {
			//		col2.collidePosition = player_lHand;
			//		thread_1_headHitted.rightHeadHitted = true;
			//		//player_hp[thread_id.thread_num].playerHp = player_hp[thread_id.thread_num].playerHp - 10.0f;
			//	}
			//	col2.lHand2Spine = checkcollition(lHand_obb[1], Spine_obb[0], 16);
			//	if (col2.lHand2Spine)
			//		col2.collidePosition = player_lHand;

			//}

			//recv_attackAnddefend[thread_id.thread_num - 1].rightGuard = true;

			retval = send(thread_client_sock, (char*)&thread_num_1_player, sizeof(thread_num_1_player), 0);
			
			//retval = send(thread_client_sock, (char*)&col1, sizeof(col1), 0);
			//cout << "collide _ position - " << col2.collidePosition.x << " " << col2.collidePosition.y << " " << col2.collidePosition.z << endl;
			/*cout << "thread_2 of thread_1 value = " << thread_id.thread_num << " / " << thread_num_1_player.player_world._41 <<
				" ," << thread_num_1_player.player_world._42 << ", " << thread_num_1_player.player_world._43 << endl;*/
			
			retval = send(thread_client_sock, (char*)&recv_attackAnddefend_1, sizeof(recv_attackAnddefend_1), 0);
			
			//retval = send(thread_client_sock, (char*)&thread_1_headHitted, sizeof(thread_1_headHitted), 0);
			
			retval = send(thread_client_sock, (char*)&thread_num_1_HP, sizeof(thread_num_1_HP), 0);

			//충돌좌표 초기화
			col2.collidePosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
			thread_1_headHitted.leftHeadHitted = false;
			thread_1_headHitted.rightHeadHitted = false;
			thread_1_headHitted.straightHtitted = false;

		}

		LeaveCriticalSection(&cs);


	}
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

void SetOBB(BoundingOrientedBox* obb, Thread_id id, const XMFLOAT3& center, const XMFLOAT3& extents, const XMFLOAT4& orientation) {
	obb[id.thread_num - 1].Center = center;
	obb[id.thread_num - 1].Extents = extents;
	obb[id.thread_num - 1].Orientation = orientation;
}
bool checkcollition(BoundingOrientedBox& first_obb, BoundingOrientedBox& second_obb, int i) {
	if (first_obb.Intersects(second_obb)) {
		//cout << i << " - COLLIDE! " << endl;
		//충돌됨
		return true;
	}
	else {
		//cout << "NOT  COLLIDE! " << endl;
		return false;
	}
}


void setPosition(XMFLOAT3& fl3x3, XMFLOAT4X4& fl4x4) {
	fl3x3.x = fl4x4._41;
	fl3x3.y = fl4x4._42;
	fl3x3.z = fl4x4._43;

}