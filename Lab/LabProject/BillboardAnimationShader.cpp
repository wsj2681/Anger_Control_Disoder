#include "stdafx.h"
#include "BillboardAnimationShader.h"
#include "Texture.h"
#include "Scene.h"

// Texture : rootParameterIndex = 21
// KeyFrame : rootParameterINdex = 22

extern Scene* gScene;

BillboardAnimationShader::BillboardAnimationShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	BuildObjects(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, filePath);
}

BillboardAnimationShader::~BillboardAnimationShader()
{
}

void BillboardAnimationShader::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(VS_CB_ANIMATION_INFO) + 255) & ~255); //256ÀÇ ¹è¼ö
	m_pd3dcbkeyFrameInfo = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbkeyFrameInfo->Map(0, NULL, (void**)&m_pcbMappedkeyFrameInfo);
}


void BillboardAnimationShader::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	::memcpy(&m_pcbMappedkeyFrameInfo->keyFrame, &gScene->bill->keyFrame, sizeof(unsigned int));
	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbkeyFrameInfo->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(22, d3dGpuVirtualAddress);
}

void BillboardAnimationShader::ReleaseShaderVariables()
{
	if (m_pd3dcbkeyFrameInfo)
	{
		m_pd3dcbkeyFrameInfo->Unmap(0, NULL);
		m_pd3dcbkeyFrameInfo->Release();
	}
}

D3D12_INPUT_LAYOUT_DESC BillboardAnimationShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE BillboardAnimationShader::CreateVertexShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSBillBoardAnimation", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_SHADER_BYTECODE BillboardAnimationShader::CreatePixelShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSBillBoardAnimation", "ps_5_1", &m_pd3dPixelShaderBlob));
}

void BillboardAnimationShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	//m_pTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
	//m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filePath, 0);
	//Scene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 21, false);
}
//
//void BillboardAnimationShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
//{
//	if (active)
//	{
//		m_pTexture->UpdateShaderVariables(pd3dCommandList);
//		Shader::Render(pd3dCommandList, pCamera);
//
//		pd3dCommandList->DrawInstanced(6, 1, 0, 0);
//	}
//}
