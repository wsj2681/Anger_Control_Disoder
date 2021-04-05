#pragma once
#include "StandardShader.h"

class Object;

class StandardObjectsShader : public StandardShader
{
public:
	StandardObjectsShader();
	virtual ~StandardObjectsShader();

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, ModelInfo* pModel, void* pContext = NULL);
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void ReleaseObjects();

	virtual void ReleaseUploadBuffers();

	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

protected:
	Object** m_ppObjects = 0;
	int								m_nObjects = 0;
};
