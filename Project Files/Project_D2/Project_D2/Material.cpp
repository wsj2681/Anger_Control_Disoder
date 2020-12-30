#include "stdafx.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

CMaterial::CMaterial()
{
}

CMaterial::~CMaterial()
{
	if (m_pTexture) m_pTexture->Release();
	if (m_pShader) m_pShader->Release();
}

void CMaterial::SetShader(CShader* pShader)
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();
}

void CMaterial::SetTexture(CTexture* pTexture)
{
	if (m_pTexture) m_pTexture->Release();
	m_pTexture = pTexture;
	if (m_pTexture) m_pTexture->AddRef();
}

void CMaterial::ReleaseUploadBuffers()
{
	//	if (m_pShader) m_pShader->ReleaseUploadBuffers();
	if (m_pTexture) m_pTexture->ReleaseUploadBuffers();
}

void CMaterial::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 4, &m_xmf4AmbientColor, 16);
	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 4, &m_xmf4AlbedoColor, 20);
	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 4, &m_xmf4SpecularColor, 24);
	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 4, &m_xmf4EmissiveColor, 28);

	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 1, &m_nType, 32);

	if (m_pTexture) m_pTexture->UpdateShaderVariables(pd3dCommandList);
}

void CMaterial::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList, ID3D12Resource* pd3dResource, CB_GAMEOBJECT_INFO* pMappedObj)
{
	XMStoreFloat4(&pMappedObj->m_xmf4AmbientColor, XMLoadFloat4(&m_xmf4AmbientColor));
	XMStoreFloat4(&pMappedObj->m_xmf4AlbedoColor, XMLoadFloat4(&m_xmf4AlbedoColor));
	XMStoreFloat4(&pMappedObj->m_xmf4SpecularColor, XMLoadFloat4(&m_xmf4SpecularColor));
	XMStoreFloat4(&pMappedObj->m_xmf4EmissiveColor, XMLoadFloat4(&m_xmf4EmissiveColor));
	pMappedObj->m_nType = m_nType;

	//D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbCamera->GetGPUVirtualAddress();
	//pd3dCommandList->SetGraphicsRootConstantBufferView(0, d3dGpuVirtualAddress);

	//pd3dCommandList->SetGraphicsRootConstantBufferView();
}

void CMaterial::ReleaseShaderVariables()
{
	if (m_pShader) m_pShader->ReleaseShaderVariables();
	if (m_pTexture) m_pTexture->ReleaseShaderVariables();
}