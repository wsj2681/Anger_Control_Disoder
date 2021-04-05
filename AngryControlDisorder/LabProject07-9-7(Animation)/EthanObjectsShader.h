#pragma once
#include "SkinnedAnimationObjectsShader.h"
class CEthanObjectsShader : public CSkinnedAnimationObjectsShader
{
public:
	CEthanObjectsShader();
	virtual ~CEthanObjectsShader();

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, ModelInfo* pModel, void* pContext = NULL);
};