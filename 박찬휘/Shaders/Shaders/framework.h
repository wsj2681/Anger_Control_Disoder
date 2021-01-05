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
#include <fstream>
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
#include "Preprocs.h"

#define FRAME_BUFFER_WIDTH	1920
#define FRAME_BUFFER_HEIGHT	1080

extern UINT descriptorIncrementSize;

// temporary class
//TODO : Camera
class Camera;

//TODO : Texture
class Texture {
public:
	Texture() = default;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	~Texture() = default;
public:
	int GetTextures() { return 0; }
	UINT GetTextureType() { return 0; }
	ID3D12Resource* GetResource(int index) { return nullptr; }
	D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(int index) { return D3D12_SHADER_RESOURCE_VIEW_DESC(); }
	int GetRootParameters() { return 0; }

	void SetGpuDescriptorHandle(int index, D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorNextHandle) {}
	void SetRootParameterIndex(int index, int rootParameterIndex) {}
};

//TODO : GameObject
class GameObject;

//TODO : LoadedModelInfo
class LoadedModelInfo;