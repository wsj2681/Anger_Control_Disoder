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

#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
using namespace std;




const char* host = "localhost";
const char* user = "root";
const char* pw = "1234";
const char* db = "shopdb";


// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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
// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		//���� �ϋ�
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		//���̻� �����͸� �޾ƿ��� ���ҋ�
		else if (received == 0)
			break;
		// ������ ����� 
		left -= received; //���� �����;�
		ptr += received;  //�о�� ������
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
	// ip�ּҿ� ��Ʈ��ȣ ���
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	//��ɾ� ���� �Է¹ޱ�
	int orderLen;
	retval = recvn(client_sock, (char*)&orderLen, sizeof(orderLen), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return 0;;
	}

	int bufCount = 0; // ���� �ݺ��� ���� ����  i
	int strSave = 0; // ���� ���� �κ��� ���� ���� ����  j
	int bufSizeSet = 0; // �ݺ����� ���ۻ����� �ʱ�ȭ�� ���� ����   g

	//��ɾ�  �ޱ�
	char orderName[128];
	ZeroMemory(&orderName, sizeof(orderName));
	
	int totalsize;
	while (true) {
		retval = recvn(client_sock, buf, orderLen, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			return 0;
		}
		//�� ���� �����Ͱ� ������
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

	cout << "���� ��ɾ�  : " << orderName << endl;

	
	if (totalsize == orderLen)
		cout << "\n" << orderName << "�� ���������� ������ �޾ҽ��ϴ�" << endl;
	else
		cout << "\n" << orderName << "�� ������ ����� ���� ���߽��ϴ�" << endl;

	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	
	//�����ͺ��̽� ����
	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL) //  ���� ���� ó��
	{
		printf("%d ���� : %s, %d\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}
	else
	{
		printf("���� ����\n"); //���Ἲ�� �޼��� ���

		if (mysql_select_db(&conn, db)) // �����ͺ��̽� ����
		{
			printf("%d ���� : %s, %d\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;


		}
		//printf("%d���� : %s, %d\n");
		const char* query = "select * from membertbl";
		int state = 0;

		const char* Update = "update membertbl set ";
		const char* Deletedatabase = "delete from membertbl where";

		int UserSelect = 0;

		if (UserSelect == 0) {
			state = mysql_query(connection, query);
			if (state == 0) {
				sql_result = mysql_store_result(connection);	// Result set�� ����
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL) // Result set ���� 1���� �迭�� ������
				{
					printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);	// ����� �迭�� ���
				}
				mysql_free_result(sql_result); // Result set ����
			}
		}
		else if (UserSelect == 1) {
			state = mysql_query(connection, Update);
			if (state == 0) {
				sql_result = mysql_store_result(connection);	// Result set�� ����
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL) // Result set ���� 1���� �迭�� ������
				{
					printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);	// ����� �迭�� ���
				}
				mysql_free_result(sql_result); // Result set ����
			}
		}
		else if (UserSelect == 2) {
			state = mysql_query(connection, Deletedatabase);
			if (state == 0) {
				sql_result = mysql_store_result(connection);	// Result set�� ����
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL) // Result set ���� 1���� �迭�� ������
				{
					printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);	// ����� �迭�� ���
				}
				mysql_free_result(sql_result); // Result set ����
			}
		}

		mysql_close(connection); //db���� ����
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

	// ���� �ʱ�ȭ
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

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	int len;

	HANDLE hThread;

	cout << "���� ������~~~" << endl;


	


	while (1) {

		// Ŭ���̾�Ʈ ���� ����
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		cout << "\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�= " << inet_ntoa(clientaddr.sin_addr) // Ŭ���̾�Ʈ �ּ� ��Ʈ��ũ ����Ʈ ����
			<< ", ��Ʈ ��ȣ=" << ntohs(clientaddr.sin_port) << endl; // ��Ʈ��ȣ ��Ʈ��ũ����Ʈ ����

		//     �������ڵ�, ����ũ��, �����Լ�, �����Լ��� ���� , ��������� �ɼ�, ������ID����
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

	// ���� ����
	WSACleanup();
	return 0;

}