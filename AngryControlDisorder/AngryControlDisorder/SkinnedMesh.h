#pragma once
#include "StandardMesh.h"

#define SKINNED_ANIMATION_BONES		128

class Object;

class SkinnedMesh : public StandardMesh
{
public:

	SkinnedMesh();
	SkinnedMesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	virtual ~SkinnedMesh();

protected:

public:

	void PrepareSkinning(Object* pModelRootObject);
	void LoadSkinInfoFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void ReleaseUploadBuffers();

	virtual void OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, void* pContext);
};

