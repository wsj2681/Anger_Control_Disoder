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

	int	bonesPerVertex = 4;

	XMINT4* boneIndices = nullptr;
	XMFLOAT4* boneWeights = nullptr;

	ID3D12Resource* boneIndexBuffer = nullptr;
	ID3D12Resource* boneIndexUploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW boneIndexBufferView{};

	ID3D12Resource* boneWeightBuffer = nullptr;
	ID3D12Resource* boneWeightUploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW boneWeightBufferView{};

public:

	int m_nSkinningBones = 0;

	char(*skinningBoneNames)[64];
	Object** skinningBoneFrameCaches = nullptr; //[m_nSkinningBones]

	XMFLOAT4X4* bm_pxmf4x4BindPoseBoneOffsets = nullptr; //Transposed

	ID3D12Resource* cbBindPoseBoneOffsets = nullptr;
	XMFLOAT4X4* mappedBindPoseBoneOffsets = nullptr;

	ID3D12Resource* cbSkinningBoneTransforms = nullptr;
	XMFLOAT4X4* cbMappedSkinningBoneTransforms = nullptr;

public:

	void PrepareSkinning(Object* pModelRootObject);
	void LoadSkinInfoFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void ReleaseUploadBuffers();

	virtual void OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, void* pContext);
};

