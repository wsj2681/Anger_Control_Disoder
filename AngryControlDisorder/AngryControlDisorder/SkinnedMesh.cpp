#include "framework.h"
#include "SkinnedMesh.h"
#include "Object.h"

SkinnedMesh::SkinnedMesh()
{
	
}

SkinnedMesh::SkinnedMesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList) :SkinnedMesh()
{
}

SkinnedMesh::~SkinnedMesh()
{
}

void SkinnedMesh::PrepareSkinning(Object* pModelRootObject)
{
}

void SkinnedMesh::LoadSkinInfoFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile)
{
}

void SkinnedMesh::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void SkinnedMesh::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void SkinnedMesh::ReleaseShaderVariables()
{
}

void SkinnedMesh::ReleaseUploadBuffers()
{
}

void SkinnedMesh::OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, void* pContext)
{
}
