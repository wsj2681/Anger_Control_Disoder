#pragma once
#include "SkinnedAnimationObjectsShader.h"

class ModelInfo;

class BoxerObjectsShader : public CSkinnedAnimationObjectsShader
{
public:
	BoxerObjectsShader();
	virtual ~BoxerObjectsShader();

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, ModelInfo* pModel, void* pContext = NULL);
};