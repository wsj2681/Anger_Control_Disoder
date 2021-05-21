#pragma once
#include "StandardMesh.h"
#define SKINNED_ANIMATION_BONES		128

class Object;

class SkinnedMesh : public CStandardMesh
{
public:
	SkinnedMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~SkinnedMesh();

protected:
	int								m_nBonesPerVertex = 4;

	XMINT4* m_pxmn4BoneIndices = nullptr;
	XMFLOAT4* m_pxmf4BoneWeights = nullptr;

	ID3D12Resource* m_pd3dBoneIndexBuffer = nullptr;
	ID3D12Resource* m_pd3dBoneIndexUploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dBoneIndexBufferView;

	ID3D12Resource* m_pd3dBoneWeightBuffer = nullptr;
	ID3D12Resource* m_pd3dBoneWeightUploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dBoneWeightBufferView;

public:
	int								m_nSkinningBones = 0;

	char(*m_ppstrSkinningBoneNames)[64];
	Object** m_ppSkinningBoneFrameCaches = nullptr; //[m_nSkinningBones]

	XMFLOAT4X4* m_pxmf4x4BindPoseBoneOffsets = nullptr; //Transposed

	ID3D12Resource* m_pd3dcbBindPoseBoneOffsets = nullptr;
	XMFLOAT4X4* m_pcbxmf4x4MappedBindPoseBoneOffsets = nullptr;

	ID3D12Resource* m_pd3dcbSkinningBoneTransforms = nullptr;
	XMFLOAT4X4* m_pcbxmf4x4MappedSkinningBoneTransforms = nullptr;

public:
	void PrepareSkinning(Object* pModelRootObject);
	void LoadSkinInfoFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void ReleaseUploadBuffers();

	virtual void OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, void* pContext);
};