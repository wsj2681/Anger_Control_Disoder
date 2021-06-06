#pragma once
#include "Object.h"
class CubeObject : public Object
{
public:
	CubeObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	CubeObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, const float& x, const float& y, const float& z);
	CubeObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, const float& x, const float& y, const float& z, const char* name);
	virtual ~CubeObject();
	virtual void Update(const float& fElapsedTime, Object* bone);
};

