#pragma once
#include "Object.h"
class PlaneObject : public Object
{
public:
	PlaneObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath, bool isDDS = true);
	virtual ~PlaneObject();

public:

	bool isActive = false;

public:
	void SetLookAt(XMFLOAT3& target);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera);
};