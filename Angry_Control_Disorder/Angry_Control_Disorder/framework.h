#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN

// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더파일
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <string.h>

// C++ 런타임 헤더파일
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// TCP/IP 런타임 헤더파일
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
//#define _WITH_SERVER_CONNECT

// DirectX12 런타임 헤더파일
#include <wrl.h>
#include <shellapi.h>

#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

#include <Mmsystem.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

using namespace DirectX::PackedVector;
using Microsoft::WRL::ComPtr;


#include "Math.h"

// Defines
constexpr int FRAME_BUFFER_WIDTH = 1280;
constexpr int FRAME_BUFFER_HEIGHT = 720;
#define _WITH_CB_WORLD_MATRIX_DESCRIPTOR_TABLE

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

#define RELEASE(x) if((x)) (x)->Release(); (x) = nullptr


// Global Functions

extern ID3D12Resource* CreateBufferResource(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void* pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType = D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, ID3D12Resource * *ppd3dUploadBuffer = NULL);
extern ID3D12Resource* CreateTextureResourceFromDDSFile(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, wchar_t* pszFileName, ID3D12Resource * *ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
extern ID3D12Resource* CreateTextureResourceFromWICFile(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, wchar_t* pszFileName, ID3D12Resource * *ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
extern BYTE ReadStringFromFile(FILE * pInFile, char* pstrToken);
extern int ReadIntegerFromFile(FILE * pInFile);
extern float ReadFloatFromFile(FILE * pInFile);

// Global Values
extern UINT gnCbvSrvDescriptorIncrementSize;