// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#define _CRT_SECURE_NO_WARNINGS
//#define _WITH_SERVER_CONNECT			// 서버 연결및 해제
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#define _WITH_ANIMATION_SRT
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
#include <map>
/////////////SEVER///////////
#include <thread>
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

extern UINT gnCbvSrvDescriptorIncrementSize;

extern ID3D12Resource *CreateBufferResource(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType = D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, ID3D12Resource **ppd3dUploadBuffer = nullptr);
extern ID3D12Resource *CreateTextureResourceFromDDSFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, wchar_t *pszFileName, ID3D12Resource **ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
extern ID3D12Resource *CreateTextureResourceFromWICFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, wchar_t *pszFileName, ID3D12Resource **ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

extern BYTE ReadStringFromFile(FILE *pInFile, char *pstrToken);
extern int ReadIntegerFromFile(FILE *pInFile);
extern float ReadFloatFromFile(FILE *pInFile);

//----------------기본 애니메이션-------------------
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
constexpr UINT ANIMATION_KNOCKDOWN = 0x16;          // Knockdown
constexpr UINT ANIMATION_KNOCKDOWNED = 0x17;        // Knockdowned
constexpr UINT ANIMATION_GUARD_LEFT_HEAD = 0x18;	// Close_One_Block_L
constexpr UINT ANIMATION_GUARD_RIGHT_HEAD = 0x19;	// Close_One_Block_R
constexpr UINT ANIMATION_GUARD_BODY = 0x1A;			// Close_Both_Block
constexpr UINT ANIMATION_GUARD_LOW_LEFT = 0x61;     // Low Block L
constexpr UINT ANIMATION_GUARD_LOW_RIGHT = 0x62;    // Low Block R
constexpr UINT ANIMATION_GUARD_SIDE_LEFT = 0x63;    // Side Block L
constexpr UINT ANIMATION_GUARD_SIDE_RIGHT = 0x64;   // Side Block R
constexpr UINT ANIMATION_GUARD_HOOK_LEFT = 0x65;    // Hook Block L
constexpr UINT ANIMATION_GUARD_HOOK_RIGHT = 0x66;   // Hook Block R
//----------------공격 애니메이션-------------------
constexpr UINT ANIMATION_HOOK_L = 0x1B; 			// Hook_L 
constexpr UINT ANIMATION_HOOK_R = 0x1C;				// Hook_R
constexpr UINT ANIMATION_JAB = 0x1D;				// Jab
constexpr UINT ANIMATION_CROSS = 0x1E;              // Cross
constexpr UINT ANIMATION_CROSS_BODY = 0x1F;         // Cross_Body
constexpr UINT ANIMATION_ONE_TWO = 0x20;            // One_Two
constexpr UINT ANIMATION_LEFT_BODY_HOOK = 0x21;     // Left Body Hook(liver punch)
constexpr UINT ANIMATION_RIGHT_BODY_HOOK = 0x22;    // Right_BodyHook(Spleen Punch)
constexpr UINT ANIMATION_UPPER_CUT_L = 0x23;        // UpperCut_L
constexpr UINT ANIMATION_UPPER_CUT_R = 0x24;        // UpperCut_R
constexpr UINT ANIMATION_1_2_3_KICK = 0x25;			// 1_2_3_Kick
constexpr UINT ANIMATION_1_2_KICK = 0x26;			// 1_2_Kick
constexpr UINT ANIMATION_CROSS_KICK = 0x27;			// Cross_Kick
constexpr UINT ANIMATION_DIAGONAL_ELBOW_L = 0x28;	// Diagonal_Elbow_L
constexpr UINT ANIMATION_DIAGONAL_ELBOW_R = 0x29;	// Diagonal_Elbow_R
constexpr UINT ANIMATION_DOWNWARD_ELBOW_R = 0x2A;	// Downward_Elbow_R
constexpr UINT ANIMATION_ELBOW_L = 0x2B;			// Elbow_L
constexpr UINT ANIMATION_ELBOW_R = 0x2C;			// Elbow_R
constexpr UINT ANIMATION_UPPER_ELBOW_L = 0x2D;		// Upper_Elbow_L
constexpr UINT ANIMATION_UPPER_ELBOW_R = 0x2E;		// Upper_Elbow_R
constexpr UINT ANIMATION_AXE_KICK_R = 0x2F;			// Axe_Kick_R
constexpr UINT ANIMATION_CRANE_KICK_R = 0x30;		// Crane_Kick_R
constexpr UINT ANIMATION_KICK_HIGH_L = 0x31;		// Kick_High_L
constexpr UINT ANIMATION_KICK_MID_L = 0x32;			// Kick_Mid_L
constexpr UINT ANIMATION_LOW_KICK_360_R = 0x33;		// Low_Kick_360_R
constexpr UINT ANIMATION_LOW_KICK_R = 0x34;			// Low_Kick_R
constexpr UINT ANIMATION_LOW_KICK_SL = 0x35;		// Low_Kick_Shortened (KickBoxing Style)_L
constexpr UINT ANIMATION_MAEGERI_BODY_R = 0x36;		// MaeGeri_Body(Karate Kick)_R
constexpr UINT ANIMATION_MAEGERI_HEAD_R = 0x37;		// MaeGeri_Head(Karate Kick)_R
constexpr UINT ANIMATION_MIKAZUKIGERI_R = 0x38;		// MikazukiGeri(Karate)_R
constexpr UINT ANIMATION_PUSH_KICK_R = 0x39;		// Push_Kick (Muat Thai)_R
constexpr UINT ANIMATION_SHORT_LOW_KICK_L = 0x3A;	// Short_Low_Kick_L
constexpr UINT ANIMATION_SIDE_KICK_L = 0x3B;		// Side_Kick_L
constexpr UINT ANIMATION_SIDE_KICK_R = 0x3C;		// Side_Kick_R
constexpr UINT ANIMATION_URAMIKAZUKIGERI_R = 0x3D;	// UraMikazukiGeri(Karate)_R
constexpr UINT ANIMATION_HIT_HEAD_STRIGHT_B = 0x3E; // Hit_Head_Stright_B
//----------------축 이동 애니메이션-------------------
constexpr UINT ANIMATION_JAB_KICK = 0x3F;			// Jab_Kick
constexpr UINT ANIMATION_KICK_COMBO = 0x40;			// Kick_Combo
constexpr UINT ANIMATION_KICK_COMBO_HEAD = 0x41;	// Kick_Combo_Head
constexpr UINT ANIMATION_KICK_COMBO_TORSO = 0x42;	// Kick_Combo_Torso
constexpr UINT ANIMATION_STEP_DOUBLEJAB_CROSS = 0x43;	// Step_DoubleJab_Cross
constexpr UINT ANIMATION_FAKE_CROSS_R_KICK_HIGH = 0x44;	// Fake_Cross_R_ Kick_High
constexpr UINT ANIMATION_STEP_HOOK_L = 0x45;		// Step_Hook_L
constexpr UINT ANIMATION_STEP_JAB = 0x46;			// Step_Jab
constexpr UINT ANIMATION_HIT_HEAD_LEFT_B = 0x47;	// Hit_Head_Left_B
constexpr UINT ANIMATION_HIT_HEAD_LEFT_C = 0x48;	// Hit_Head_Left_C
constexpr UINT ANIMATION_HIT_HEAD_LEG_LEFT_B = 0x49;	// Hit_Head_Leg_Left_B
constexpr UINT ANIMATION_HIT_HEAD_LEG_LEFT_C = 0x4A;	// Hit_Head_Leg_Left_C
constexpr UINT ANIMATION_HIT_HEAD_LEG_RIGHT_B = 0x4B;	// Hit_Head_Leg_Right_B
constexpr UINT ANIMATION_HIT_HEAD_LEG_RIGHT_C = 0x4C;	// Hit_Head_Leg_Right_C
constexpr UINT ANIMATION_HIT_HEAD_RIGHT_B = 0x4D;	// Hit_Head_Right_B
constexpr UINT ANIMATION_HIT_HEAD_RIGHT_C = 0x4E;	// Hit_Head_Right_C
constexpr UINT ANIMATION_HIT_HEAD_STRIGHT_C = 0x4F;	// Hit_Head_Stright_C
constexpr UINT ANIMATION_HIT_L_LEG_IN_B = 0x50;		// Hit_L_leg_In_B
constexpr UINT ANIMATION_HIT_L_LEG_IN_C = 0x51;		// Hit_L_leg_In_C
constexpr UINT ANIMATION_HIT_L_LEG_OUT_B = 0x52;	// Hit_L_leg_Out_B
constexpr UINT ANIMATION_HIT_L_LEG_OUT_C = 0x53;	// Hit_L_leg_Out_C
constexpr UINT ANIMATION_HIT_R_LEG_OUT_B = 0x54;	// Hit_R_leg_Out_B
constexpr UINT ANIMATION_HIT_R_LEG_OUT_C = 0x55;	// Hit_R_leg_Out_C
constexpr UINT ANIMATION_HIT_TORSO_LEFT_B = 0x56;	// Hit_Torso_Left_B
constexpr UINT ANIMATION_HIT_TORSO_LEFT_C = 0x57;	// Hit_Torso_Left_C
constexpr UINT ANIMATION_HIT_TORSO_RIGHT_B = 0x58;	// Hit_Torso_Right_B
constexpr UINT ANIMATION_HIT_TORSO_RIGHT_C = 0x59;	// Hit_Torso_Right_C
constexpr UINT ANIMATION_HIT_TORSO_STRIGHT_B = 0x5A;	// Hit_Torso_Stright_B
constexpr UINT ANIMATION_HIT_TORSO_STRIGHT_C = 0x5B;	// Hit_Torso_Stright_C
constexpr UINT ANIMATION_FUL_HIGH_KIK_L = 0x5C;		// Ful_High_Kik_L
constexpr UINT ANIMATION_FUL_LOW_KIK_L = 0x5D;		// Ful_Low_KIK_L
constexpr UINT ANIMATION_FUL_MID_KIK_L = 0x5E;		// Ful_Mid_Kik_L
constexpr UINT ANIMATION_HIGH_KICK_360_R = 0x5F;	// High_Kick_360_R
constexpr UINT ANIMATION_MIDDLE_KICK_360_R = 0x60;	// Middle_Kick_360_R
//----------------------------------------------------------
constexpr UINT DAMAGE_HOOK = 10;
constexpr UINT DAMAGE_HOOK_L = 10;
constexpr UINT DAMAGE_HOOK_R = 10;
constexpr UINT DAMAGE_JAB = 7;
constexpr UINT DAMAGE_STRAIGHT = 15;
//-----------------State-----------------------------
constexpr UINT IDLE = 1;
constexpr UINT ATTACK = 2;
constexpr UINT HIT = 3;
constexpr UINT HIGH_GUARD = 4;
constexpr UINT MIDDLE_GUARD = 5;
constexpr UINT LOW_GUARD = 6;
constexpr UINT OTHER = 7;


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

#define SAFE_RELEASE(ptr) {if (ptr) ptr->Release(); ptr = nullptr; }
#define SAFE_DELETE(ptr) {if (ptr) delete ptr; ptr = nullptr; }
#define SAFE_DELETEARR(ptr) {if (ptr) delete[] ptr; ptr = nullptr; }