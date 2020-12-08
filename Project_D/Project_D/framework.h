// framework.h : 필요한 외부 헤더파일을 정의합니다.

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN

// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//C++ 관련 헤더 파일입니다.
#include <string>
#include <vector>
#include <iostream>
using namespace std;

//DirectX 관련 헤더 파일입니다.
#include <wrl.h>
#include <shellapi.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

using namespace DirectX::PackedVector;
using Microsoft::WRL::ComPtr;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

//사용자 정의 헤더 파일입니다.
#include "MathHelper.h"
#include "Defines.h"

//TODO : 여러번 정의 해결 - extern 사용
extern UINT descriptorIncrementSize{ 0 };
extern bool terrainTessellationWireFrame{ false };