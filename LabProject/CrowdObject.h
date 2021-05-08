#pragma once
#include "Object.h"
class CrowdObject : public Object
{
public:
	CrowdObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, ModelInfo* pModel, int nAnimationTracks);
	virtual ~CrowdObject();
};