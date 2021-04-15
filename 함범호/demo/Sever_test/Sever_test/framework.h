#pragma once

#define BUFFERSIZE 512
#define SERVERPORT 9000
#define BUFSIZE    512

#include <iostream>

using namespace std;

#include <WinSock2.h>
#include <thread>
#pragma comment(lib, "ws2_32")


/////////Client 헤더//////////
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>

#include <iostream>
#include <string>
#include <wrl.h>
#include <shellapi.h>

#include <fstream>
#include <vector>

#include <conio.h>

/////direct x 전용 헤더
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#include <Mmsystem.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

using namespace DirectX;
using namespace DirectX::PackedVector;

using Microsoft::WRL::ComPtr;


#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#pragma comment(lib, "dxguid.lib")

////////////////////////


// hierarchicalGameObjects.data()[1].head->GetPosition();
// hierarchicalGameObjects.data()[1].head->SetPosition(XMFLOAT3());
//Object* head	// 머리
//Object* rHand	// 오른손
//Object* lHand	// 왼손
//Object* lFoot	// 왼발
//Object* rFoot	// 오른발
//Object* spine	// 척추 중심
/////////////////////////////


#pragma pack(push,1)
struct Player_world {

	XMFLOAT4X4 player_world;

};
#pragma pack(pop)

#pragma pack(push,1)
struct Thread_id {

	int thread_num;

};
#pragma pack(pop)

#pragma pack(push,1)
struct collide {

	bool check_collide = false;

};
#pragma pack(pop)


void display_error(const char* msg, int err_no);


