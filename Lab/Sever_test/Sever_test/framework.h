#pragma once

#define BUFFERSIZE 512
#define SERVERPORT 9000
#define BUFSIZE    512

#define STATE_IDLE 0x01
#define STATE_MOVE 0x02
#define STATE_ATTACK_LEFT_HOOK 0x03
#define STATE_ATTACK_RIGHT_HOOK 0x04
#define STATE_ATTACK_JAB 0x05
#define STATE_GUARD_LEFT_HEAD 0x06
#define STATE_GUARD_RIGHT_HEAD 0x07
#define STATE_GUARD_BODY 0x08
#define STATE_HIT_TORSO_LEFT 0x08
#define STATE_HIT_TORSO_RIGHT 0x09
#define STATE_HIT_TORSO_STRIGHT 0x0A
#define STATE_CEREMONY 0x0B

#include <iostream>

using namespace std;

#include <WinSock2.h>
#include <thread>
#pragma comment(lib, "ws2_32")


/////////Client ���//////////
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

/////direct x ���� ���
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

	UINT nowState = STATE_IDLE;

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

	bool rHand2rHand = false; // �����հ����� �����տ� ����
	bool rHand2lHand = false; // �����հ����� �޼տ� ����
	bool rHand2Spine = false; // �����հ����� ô�߸� ����
	bool rHand2Head = false; // ������ ������ �밡���� ����

	bool lHand2rHand = false; // �޼հ����� �����տ� ����
	bool lHand2lHand = false; // �޼հ����� �޼տ� ����
	bool lHand2Spine = false; // �޼հ����� ô�߸� ����
	bool lHand2Head = false; // �޼۰��� �밡���� ����

	bool rFoot2Spine = false; // ��������  ô�߸� ����
	bool rFoot2Head = false; // �������� �밡���� ����

	bool lFoot2Spine = false; // �޹��� ô�߸� ����
	bool lFoot2Head = false; // �޹��� �밡���� ����

	bool headHitted = false;  // �밡���� ����
	bool spineHitted = false;  // ô�߸� ����

	XMFLOAT3 collidePosition = XMFLOAT3(0.0f, 0.0f, 0.0f);		//�浹�� ���� ���� ��ǥ

};
#pragma pack(pop)

#pragma pack(push,1)
struct AttackAndDefend {

	bool leftHand = false;
	bool rightHand = false;
	bool jap = false;

	bool hitTorsoLeft = false;
	bool hitTorsoRight = false;
	bool hitTorsoStright = false;

	bool leftGuard = false;
	bool rightGuard = false;
	bool middleGuard = false;

	bool checkAni = false;

};
#pragma pack(pop)


void display_error(const char* msg, int err_no);


