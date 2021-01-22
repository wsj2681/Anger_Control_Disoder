#include <stdio.h>
#include <winsock2.h>
#include <mysql.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iostream>
#pragma comment(lib,"libmysql.lib")

#define SERVERPORT 9000
#define BUFSIZE    512

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
using namespace std;




const char* host = "localhost";
const char* user = "root";
const char* pw = "1234";
const char* db = "shopdb";


// 소켓 함수 오류 출력 후 종료
void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		//오류 일떄
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		//더이상 데이터를 받아오지 못할떄
		else if (received == 0)
			break;
		// 데이터 존재시 
		left -= received; //남은 데이터양
		ptr += received;  //읽어온 데이터
	}

	return (len - left);
}


DWORD WINAPI ProcessClient(LPVOID arg) {


	int retval;

	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	char buf[BUFSIZE + 1];

	int len;
	// ip주소와 포트번호 담기
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	//명령어 길이 입력받기
	int orderLen;
	retval = recvn(client_sock, (char*)&orderLen, sizeof(orderLen), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return 0;;
	}

	int bufCount = 0; // 버퍼 반복을 위한 변수  i
	int strSave = 0; // 문자 넣을 부분을 위한 저장 변수  j
	int bufSizeSet = 0; // 반복문안 버퍼사이즈 초기화를 위한 변수   g

	//명령어  받기
	char orderName[128];
	ZeroMemory(&orderName, sizeof(orderName));
	
	int totalsize;
	while (true) {
		retval = recvn(client_sock, buf, orderLen, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			return 0;
		}
		//더 받을 데이터가 없을떄
		if (retval == 0) {
			return 0;
		}
		else {
			while (bufSizeSet < BUFSIZE && bufCount < orderLen) {
				orderName[strSave] = buf[bufCount];
				++bufCount;
				++strSave;
				++bufSizeSet;
			}
			bufCount = 0;
			bufSizeSet = 0;
		}
		totalsize = retval;
	}

	cout << "받은 명령어  : " << orderName << endl;

	
	if (totalsize == orderLen)
		cout << "\n" << orderName << "을 성공적으로 파일을 받았습니다" << endl;
	else
		cout << "\n" << orderName << "을 파일을 제대로 받지 못했습니다" << endl;

	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	
	//데이터베이스 연동
	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL) //  연결 에러 처리
	{
		printf("%d 에러 : %s, %d\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}
	else
	{
		printf("연결 성공\n"); //연결성공 메세지 출력

		if (mysql_select_db(&conn, db)) // 데이터베이스 선택
		{
			printf("%d 에러 : %s, %d\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;


		}
		//printf("%d에러 : %s, %d\n");
		const char* query = "select * from membertbl";
		int state = 0;

		const char* Update = "update membertbl set ";
		const char* Deletedatabase = "delete from membertbl where";

		int UserSelect = 0;

		if (UserSelect == 0) {
			state = mysql_query(connection, query);
			if (state == 0) {
				sql_result = mysql_store_result(connection);	// Result set에 저장
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL) // Result set 에서 1개씩 배열에 가져옴
				{
					printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);	// 저장된 배열을 출력
				}
				mysql_free_result(sql_result); // Result set 해제
			}
		}
		else if (UserSelect == 1) {
			state = mysql_query(connection, Update);
			if (state == 0) {
				sql_result = mysql_store_result(connection);	// Result set에 저장
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL) // Result set 에서 1개씩 배열에 가져옴
				{
					printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);	// 저장된 배열을 출력
				}
				mysql_free_result(sql_result); // Result set 해제
			}
		}
		else if (UserSelect == 2) {
			state = mysql_query(connection, Deletedatabase);
			if (state == 0) {
				sql_result = mysql_store_result(connection);	// Result set에 저장
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL) // Result set 에서 1개씩 배열에 가져옴
				{
					printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);	// 저장된 배열을 출력
				}
				mysql_free_result(sql_result); // Result set 해제
			}
		}

		mysql_close(connection); //db서버 종료
	}


	// closesocket()
   // closesocket(client_sock);

}

int main() {

	////printf("MYSQL client version : %s\n", mysql_get_client_info);

	//MYSQL mysql;
	//mysql_init(&mysql);

	//if(!mysql_real_connet(&mysql, "" ))

	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	int len;

	HANDLE hThread;

	cout << "서버 구동중~~~" << endl;


	


	while (1) {

		// 클라이언트 접속 받음
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		cout << "\n[TCP 서버] 클라이언트 접속: IP 주소= " << inet_ntoa(clientaddr.sin_addr) // 클라이언트 주소 네트워크 바이트 정렬
			<< ", 포트 번호=" << ntohs(clientaddr.sin_port) << endl; // 포트번호 네트워크바이트 정렬

		//     스레드핸들, 스택크기, 시작함수, 시작함수의 인자 , 스레드생성 옵션, 스레드ID저장
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) {
			closesocket(client_sock);
		}
		else {
			CloseHandle(hThread);
		}

	}

	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;

}