#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����


#include <winsock2.h>
#include <iostream>
#include <fstream>


#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512



// ���� ��� �Լ�
void err_quit(char* msg);
void err_display(char* msg);
// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char* buf, int len, int flags);
// ���� ��� ������ �Լ�
DWORD WINAPI ClientMain(LPVOID arg);

SOCKET sock; // ����
char buf[BUFSIZE + 1]; // ������ �ۼ��� ����
HANDLE hReadEvent, hWriteEvent; // �̺�Ʈ
HWND hSendButton; // ������ ��ư
HWND hEdit1, hEdit2; // ���� ��Ʈ��

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hwnd;
    // �̺�Ʈ ����
    hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
    if (hReadEvent == NULL) return 1;
    hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hWriteEvent == NULL) return 1;

    // ���� ��� ������ ����
    CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

   

    // �̺�Ʈ ����
    CloseHandle(hReadEvent);
    CloseHandle(hWriteEvent);

    // closesocket()
    closesocket(sock);

    // ���� ����
    WSACleanup();
    return 0;
}


int main(int argc, char* argv[])
{
    int retval;

    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket()
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket()");

    // connect()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect()");


    //���� �̸� �Է¹ޱ�
    char fileN[256];
    ZeroMemory(fileN, sizeof(fileN));
    char inputName[256];
    ZeroMemory(inputName, sizeof(inputName));

    cout << "������ ������ ���̽� ��ɾ ���ּ���: ";
    string orderName;
    cin >> orderName;
    strcpy(fileN, orderName.c_str());


    //��ɾ� ����
    int orderLen = strlen(fileN);


    // ������ ��ſ� ����� ����
    char buf[BUFSIZE + 1];
    int numread = 0;
    int total = 0;
    int i = 0; // ���� ���̸� ����
    int j = 0; // ����� �������� ����ϱ����ؼ�
    int g = 0; // �ݺ����� ��� �������ؼ� 


    // ��ɾ� ���� ������(���� ����)
    retval = send(sock, (char*)&orderLen, sizeof(orderLen), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
    }

    //��ɾ� ������(���� ����)
    while (true) {

        //��ɾ ���ۿ� ���
        while (g <= BUFSIZE && i < orderLen) {
            buf[j] = fileN[i];
            ++numread;
            ++i;
            ++j;
            ++g;
        }
        //��ɾ� �������� ������
        if (numread > 0) {
            retval = send(sock, buf, numread, 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
            total += numread;
            j = 0;
            g = 0;
            numread = 0;
        }
        else if (numread == 0 && total == orderLen) {

            cout << "��ɾ� ������ �Ϸ��߽��ϴ�" << endl;
            break;
        }
    }



    closesocket(sock);
    //MessageBox(NULL, "���� ���� �Ϸ�!", "�˸�", MB_OK);
    // ���� ����
    WSACleanup();
    return 0;
}
