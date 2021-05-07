// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#define _WITH_SERVER_CONNECT			// 서버 연결및 해제
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


#define FRAME_BUFFER_WIDTH		1280
#define FRAME_BUFFER_HEIGHT		720
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

constexpr UINT ANIMATION_TYPE_ONCE = 0x01;
constexpr UINT ANIMATION_TYPE_LOOP = 0x02;
constexpr UINT ANIMATION_TYPE_PINGPONG = 0x03;

constexpr UINT ANIMATION_TRACK = 12;

//----------------------------------------------------------
// %애니메이션 추가
//----------------------------------------------------------
#define ANIMATION_IDLE 0x00					// Combat_Mode_A
#define ANIMATION_ATTACK_LOOP 0x01			// Beating_1-2(loop)
#define ANIMATION_MOVE_FORWARD 0x02			// 1_Steps_Forward
#define ANIMATION_MOVE_BACKWARD 0x03		// 1_Steps_Backward
#define ANIMATION_MOVE_LEFT 0x04			// 1_Steps_Left
#define ANIMATION_MOVE_RIGHT 0x05			// 1_Steps_Right
#define ANIMATION_HOOK_L 0x06				// Hook_L 
#define ANIMATION_HOOK_R 0x07				// Hook_R
#define ANIMATION_JAB 0x08					// Jab
#define ANIMATION_GUARD_LEFT_HEAD 0x09		// Close_One_Block_L
#define ANIMATION_GUARD_RIGHT_HEAD 0x0A		// Close_One_Block_R
#define ANIMATION_GUARD_BODY 0x0B			// Close_Both_Block
#define ANIMATION_CEREMONY 0x0C				// ceremony
#define ANIMATION_HIT_TORSO_LEFT_A 0x0D		// Hit_Torso_Left_A
#define ANIMATION_HIT_TORSO_RIGHT_A 0x0E	// Hit_Torso_Right_A
#define ANIMATION_HIT_TORSO_STRIGHT_A 0x0F	// Hit_Torso_Stright_A
#define ANIMATION_GROGI	0x10
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