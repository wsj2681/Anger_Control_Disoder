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

/////////////////////////////


#pragma pack(push,1)
struct Player_world {

	XMFLOAT4X4 player_world;

	XMFLOAT4X4 player_Head;
	XMFLOAT4X4 player_rHand;
	XMFLOAT4X4 player_lHand;
	XMFLOAT4X4 player_rFoot;
	XMFLOAT4X4 player_lFoot;
	XMFLOAT4X4 player_Spine;

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
	
	bool rHand2rHand = false;
	bool rHand2lHand = false;
	bool rHand2Spine = false;
	bool rHand2Head = false;

	bool lHand2rHand = false;
	bool lHand2lHand = false;
	bool lHand2Spine = false;
	bool lHand2Head = false;

	bool rFoot2Spine = false;
	bool rFoot2Head = false;

	bool lFoot2Spine = false;
	bool lFoot2Head = false;

};
#pragma pack(pop)

#pragma pack(push,1)
struct AttackAndDefend {

	bool leftHand = false;
	bool rightHand = false;
	bool foot = false;

	bool leftGuard = false;
	bool rightGuard = false;
	bool middleGuard = false;

	bool checkAni = false;

};
#pragma pack(pop)


void display_error(const char* msg, int err_no);


