// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

// Windows 헤더 파일입니다.
#include <windows.h>

// C의 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <string.h>

#include <wrl.h>
#include <shellapi.h>

// C++ 런타임 헤더 파일입니다.
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// DirectX 런타임 헤더 파일입니다.
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

#include "Math.h" // 사용자 정의 헤더 파일

using namespace DirectX::PackedVector;

using Microsoft::WRL::ComPtr;

extern HINSTANCE ghAppInstance;

constexpr int FRAME_BUFFER_WIDTH = 1280;
constexpr int FRAME_BUFFER_HEIGHT = 720;

#define _WITH_CB_WORLD_MATRIX_DESCRIPTOR_TABLE

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

extern UINT gnCbvSrvDescriptorIncrementSize;

extern ID3D12Resource* CreateBufferResource(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, void* pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType = D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, ID3D12Resource** uploadBuffer = nullptr);
extern ID3D12Resource* CreateTextureResourceFromDDSFile(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, wchar_t* fileName, ID3D12Resource** uploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
extern ID3D12Resource* CreateTextureResourceFromWICFile(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, wchar_t* fileName, ID3D12Resource** uploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

extern BYTE ReadStringFromFile(FILE* pInFile, char* pstrToken);
extern int ReadIntegerFromFile(FILE* pInFile);
extern float ReadFloatFromFile(FILE* pInFile);

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

#define DIR_FORWARD				0x01
#define DIR_BACKWARD			0x02
#define DIR_LEFT				0x04
#define DIR_RIGHT				0x08
#define DIR_UP					0x10
#define DIR_DOWN				0x20

#define SAFE_RELEASE(x) if((x)) (x)->Release(); (x) = nullptr;
#define DELETE_CLASS(x) if((x)) delete (x); (x) = nullptr;
#define DELETE_ARRAY(x) if((x)) delete[] (x); (x) = nullptr;