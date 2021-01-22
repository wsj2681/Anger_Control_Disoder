#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지


#include <winsock2.h>
#include <iostream>
#include <fstream>


#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512



// 오류 출력 함수
void err_quit(char* msg);
void err_display(char* msg);
// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char* buf, int len, int flags);
// 소켓 통신 스레드 함수
DWORD WINAPI ClientMain(LPVOID arg);

SOCKET sock; // 소켓
char buf[BUFSIZE + 1]; // 데이터 송수신 버퍼
HANDLE hReadEvent, hWriteEvent; // 이벤트
HWND hSendButton; // 보내기 버튼
HWND hEdit1, hEdit2; // 편집 컨트롤

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hwnd;
    // 이벤트 생성
    hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
    if (hReadEvent == NULL) return 1;
    hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hWriteEvent == NULL) return 1;

    // 소켓 통신 스레드 생성
    CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

   

    // 이벤트 제거
    CloseHandle(hReadEvent);
    CloseHandle(hWriteEvent);

    // closesocket()
    closesocket(sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}


int main(int argc, char* argv[])
{
    int retval;

    // 윈속 초기화
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


    //파일 이름 입력받기
    char fileN[256];
    ZeroMemory(fileN, sizeof(fileN));
    char inputName[256];
    ZeroMemory(inputName, sizeof(inputName));

    cout << "서버에 데이터 베이스 명령어를 써주세요: ";
    string orderName;
    cin >> orderName;
    strcpy(fileN, orderName.c_str());


    //명령어 길이
    int orderLen = strlen(fileN);


    // 데이터 통신에 사용할 변수
    char buf[BUFSIZE + 1];
    int numread = 0;
    int total = 0;
    int i = 0; // 파일 길이를 위해
    int j = 0; // 몇번쨰 버퍼인지 기억하기위해서
    int g = 0; // 반복문을 계속 돌기위해서 


    // 명령어 길이 보내기(고정 길이)
    retval = send(sock, (char*)&orderLen, sizeof(orderLen), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
    }

    //명령어 보내기(가변 길이)
    while (true) {

        //명령어를 버퍼에 담기
        while (g <= BUFSIZE && i < orderLen) {
            buf[j] = fileN[i];
            ++numread;
            ++i;
            ++j;
            ++g;
        }
        //명령어 가변으로 보내기
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

            cout << "명령어 전송을 완료했습니다" << endl;
            break;
        }
    }



    closesocket(sock);
    //MessageBox(NULL, "파일 전송 완료!", "알림", MB_OK);
    // 윈속 종료
    WSACleanup();
    return 0;
}
