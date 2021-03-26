#include "framework.h"
#include "StandardObjectShader.h"

void StandardObjectShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LoadedModelInfo* pModel, void* pContext)
{
}

void StandardObjectShader::AnimateObjects(float fTimeElapsed)
{
}

void StandardObjectShader::ReleaseObjects()
{
}

void StandardObjectShader::ReleaseUploadBuffers()
{
}

void StandardObjectShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
}
