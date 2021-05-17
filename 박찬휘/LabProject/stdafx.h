// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

//#define _WITH_SERVER_CONNECT			// 서버 연결및 해제
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#define _CRT_SECURE_NO_WARNINGS
// Windows 헤더 파일:
#include <windows.h>

// C의 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <string.h>

#include <string>
#include <wrl.h>
#include <shellapi.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <random>

/////////////SEVER///////////
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#pragma comment(lib, "ws2_32")

/////////////////////////////
using namespace std;

#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#include <Mmsystem.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#include "Math.h"

using namespace DirectX::PackedVector;

using Microsoft::WRL::ComPtr;

extern HINSTANCE						ghAppInstance;


#define FRAME_BUFFER_WIDTH		800
#define FRAME_BUFFER_HEIGHT	    600
#define _WITH_CB_WORLD_MATRIX_DESCRIPTOR_TABLE

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#pragma comment(lib, "dxguid.lib")

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

extern UINT gnCbvSrvDescriptorIncrementSize;

extern ID3D12Resource *CreateBufferResource(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType = D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, ID3D12Resource **ppd3dUploadBuffer = NULL);
extern ID3D12Resource *CreateTextureResourceFromDDSFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, wchar_t *pszFileName, ID3D12Resource **ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
extern ID3D12Resource *CreateTextureResourceFromWICFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, wchar_t *pszFileName, ID3D12Resource **ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

extern BYTE ReadStringFromFile(FILE *pInFile, char *pstrToken);
extern int ReadIntegerFromFile(FILE *pInFile);
extern float ReadFloatFromFile(FILE *pInFile);

constexpr UINT STATE_IDLE = 0x00;
constexpr UINT STATE_MOVE = 0x01;
constexpr UINT STATE_ATTACK_LEFT_HOOK = 0x02;
constexpr UINT STATE_ATTACK_RIGHT_HOOK = 0x03;
constexpr UINT STATE_ATTACK_JAB = 0x04;
constexpr UINT STATE_GUARD_LEFT_HEAD = 0x05;
constexpr UINT STATE_GUARD_RIGHT_HEAD = 0x06;
constexpr UINT STATE_GUARD_BODY = 0x07;
constexpr UINT STATE_HIT_TORSO_LEFT = 0x08;
constexpr UINT STATE_HIT_TORSO_RIGHT = 0x09;
constexpr UINT STATE_HIT_TORSO_STRIGHT = 0x0A;
constexpr UINT STATE_CEREMONY = 0x0B;
constexpr UINT STATE_KNOCKDOWN = 0x0C;

//----------------------------------------------------------
// %애니메이션 추가
//----------------------------------------------------------
constexpr UINT ANIMATION_IDLE_A = 0x00;             // Idle_A (서있는 IDLE)
constexpr UINT ANIMATION_IDLE_B = 0x01;             // Idle_B (딴짓하는 IDLE)
constexpr UINT ANIMATION_IDLE_COMBAT = 0x02;        // Idle_Combat (전투 준비)
constexpr UINT ANIMATION_COMBAT_IDLE = 0x03;        // Combat_Idle (전투 해제)
constexpr UINT ANIMATION_COMBAT_MODE_A = 0x04;      // Combat_Mode_A (전투 IDLE)
constexpr UINT ANIMATION_CLAP_LEG = 0x05;           // Clap_Leg (무릎 자학)
constexpr UINT ANIMATION_CLAP_ON_ARM = 0x06;        // Clap_On_Arm (팔 자학)
constexpr UINT ANIMATION_CLAP_ON_HEAD = 0x07;       // Clap_On_Head (머리 자학)
constexpr UINT ANIMATION_COME_HERE_1HAND = 0x08;    // Come_Here_1Hand (한손 도발)
constexpr UINT ANIMATION_COME_HERE_2HANDS = 0x09;   // Come_Here_2Hands (두손 도발)
constexpr UINT ANIMATION_COME_HERE_BRUCE_LI = 0x0A; // Come_Here_Bruce_Li (이소룡 도발)
constexpr UINT ANIMATION_KILL_YOU = 0x0B;           // Kill_You (킬유 도발)
constexpr UINT ANIMATION_RIZE_AUDITORY = 0x0C;      // Rize_Auditory (관객 호응 유도)
constexpr UINT ANIMATION_KARATE_FINISH = 0x0D;      // Karate_Finish (가라테 피니시)
constexpr UINT ANIMATION_OSSUUU = 0x0E;             // OSSUUU (OSSUUU)
constexpr UINT ANIMATION_CEREMONY = 0x0F;           // Ceremony (커스텀 세레머니)
constexpr UINT ANIMATION_WINN_BATTLE = 0x10;        // Winn_battle (승리 모션)
constexpr UINT ANIMATION_LOSTING_BATTLE = 0x11;     // Losting_Battle (패배 모션)
constexpr UINT ANIMATION_MOVE_FORWARD = 0x12;       // 1_Steps_Forward
constexpr UINT ANIMATION_MOVE_BACKWARD = 0x13;      // 1_Steps_Backward
constexpr UINT ANIMATION_MOVE_LEFT = 0x14;			// 1_Steps_Left 
constexpr UINT ANIMATION_MOVE_RIGHT = 0x15;			// 1_Steps_Right
constexpr UINT ANIMATION_HOOK_L = 0x16; 			// Hook_L 
constexpr UINT ANIMATION_HOOK_R = 0x17;				// Hook_R
constexpr UINT ANIMATION_JAB = 0x18;				// Jab
constexpr UINT ANIMATION_GUARD_LEFT_HEAD = 0x19;	// Close_One_Block_L
constexpr UINT ANIMATION_GUARD_RIGHT_HEAD = 0x1A;	// Close_One_Block_R
constexpr UINT ANIMATION_GUARD_BODY = 0x1B;			// Close_Both_Block
constexpr UINT ANIMATION_HIT_TORSO_LEFT_A = 0x1C;	// Hit_Torso_Left_A
constexpr UINT ANIMATION_HIT_TORSO_RIGHT_A = 0x1D;	// Hit_Torso_Right_A
constexpr UINT ANIMATION_HIT_TORSO_STRIGHT_A = 0x1E;// Hit_Torso_Stright_A
constexpr UINT ANIMATION_KNOCKDOWN = 0x1F;          // Knockdown
constexpr UINT ANIMATION_KNOCKDOWNED = 0x20;        // Knockdowned
constexpr UINT ANIMATION_CROSS = 0x21;              // Cross
constexpr UINT ANIMATION_CROSS_BODY = 0x22;         // Cross_Body
constexpr UINT ANIMATION_ONE_TWO = 0x23;            // One_Two
constexpr UINT ANIMATION_LEFT_BODY_HOOK = 0x24;     // Left Body Hook(liver punch)
constexpr UINT ANIMATION_RIGHT_BODY_HOOK = 0x25;    // Right_BodyHook(Spleen Punch)
constexpr UINT ANIMATION_UPPER_CUT_L = 0x26;        // UpperCut_L
constexpr UINT ANIMATION_UPPER_CUT_R = 0x27;        // UpperCut_R
//----------------데모 애니메이션-------------------
constexpr UINT ANIMATION_BLOCKS_AND_EVASION = 0x28; // Blocks_and_Evasion
constexpr UINT ANIMATION_COMBOS = 0x29;             // Combos
constexpr UINT ANIMATION_ELBOWS = 0x2A;             // Elbows
constexpr UINT ANIMATION_GENERAL = 0x2B;            // General
constexpr UINT ANIMATION_HANDS = 0x2C;              // Hands
constexpr UINT ANIMATION_HITS = 0x2D;               // Hits
constexpr UINT ANIMATION_LEGS = 0x2E;               // Legs
constexpr UINT ANIMATION_MOVES = 0x2F;              // Moves
constexpr UINT ANIMATION_TURNS = 0x30;              // Turns
//----------------------------------------------------------
constexpr UINT DAMAGE_HOOK_L = 10;
constexpr UINT DAMAGE_HOOK_R = 9;
constexpr UINT DAMAGE_JAB = 7;
constexpr UINT DAMAGE_STRAIGHT = 15;
//----------------------------------------------------------

//HR
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x) \
      if (FAILED((x))) \
      { \
         LPVOID errorLog = nullptr; \
         FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, \
            nullptr, (x), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
            reinterpret_cast<LPWSTR>(&errorLog), 0, nullptr); \
         fprintf(stderr, "%s", static_cast<char*>(errorLog)); \
         LocalFree(errorLog); \
         __debugbreak(); \
      }
#endif
#else
#ifndef HR
#define   HR(x) (x);
#endif
#endif

//ASSERT
#if defined(DEBUG) | defined(_DEBUG)
#ifndef ASSERT
#define ASSERT(expr, msg) \
      if (!(expr)) \
      { \
         fprintf(stderr, "%s, %s, %s(%d)", (msg), (#expr), __FILE__, __LINE__); \
         __debugbreak(); \
      }
#endif
#else
#ifndef ASSERT
#define ASSERT(expr, msg) ((void)0)
#endif
#endif