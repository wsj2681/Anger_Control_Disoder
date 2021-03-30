#include "stdafx.h"
#include "Shader.h"
#include "Scene.h"
#include "HPBar.h"

UIObject::UIObject(const XMFLOAT2& position, const float& width, const float& height)
{
	this->SetPosition(position);
	this->width = width;
	this->height = height;
}

XMFLOAT2& UIObject::GetPosition()
{
	return position;
}

void UIObject::SetPosition(const XMFLOAT2& position)
{
	this->position = position;
}

void UIObject::SetMaterial(int nMaterial, CMaterial* material)
{
	if (this->material[nMaterial]) material[nMaterial].Release();
	this->material[nMaterial] = material;
	if (this->material[nMaterial]) material[nMaterial].AddRef();
}

void UIObject::Update()
{
	// Update
}

void UIObject::Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* descriptorHeap, ID3D12PipelineState* uiPipelineState, D3D12_GPU_DESCRIPTOR_HANDLE srvHandle)
{
	// Render
	commandList->ClearDepthStencilView(descriptorHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	commandList->SetPipelineState(uiPipelineState);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	commandList->IASetVertexBuffers(0, 1, &uiVertexBufferView[3]);
	commandList->SetGraphicsRootDescriptorTable(1, srvHandle);
	commandList->DrawInstanced(4, 8, 0, 0);


}

void HPBar::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(VS_CB_UI_INFO) + 255) & ~255); //256ÀÇ ¹è¼ö
	m_pd3dcbHPBar = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbHPBar->Map(0, NULL, (void**)&m_pcbHPBar);
}

void HPBar::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	unsigned int hp;
	
	XMStoreInt(&hp, XMLoadInt(&this->maxHp));
	::memcpy(&m_pcbHPBar->hp, &hp, sizeof(unsigned int));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbHPBar->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(5, d3dGpuVirtualAddress);
}

//void UIObject::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
//{
//	if (m_nTextureType == RESOURCE_TEXTURE2D_ARRAY)
//	{
//		pd3dCommandList->SetGraphicsRootDescriptorTable(m_pRootArgumentInfos[0].m_nRootParameterIndex, m_pRootArgumentInfos[0].m_d3dSrvGpuDescriptorHandle);
//	}
//	else
//	{
//		for (int i = 0; i < m_nTextures; i++)
//		{
//			pd3dCommandList->SetGraphicsRootDescriptorTable(m_pRootArgumentInfos[i].m_nRootParameterIndex, m_pRootArgumentInfos[i].m_d3dSrvGpuDescriptorHandle);
//		}
//	}
//}

HPBar::HPBar(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* rootSignature)
{
	CreateShaderVariables(device, commandList);

	CTexture* hpBarTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	hpBarTexture->LoadTextureFromFile(device, commandList, L"UI/hpbar.png", 0);

	HPUIShader* hpuishader = new HPUIShader();
	hpuishader->CreateShader(device, commandList, rootSignature);
	hpuishader->CreateShaderVariables(device, commandList, rootSignature);

	CScene::CreateShaderResourceViews(device, hpBarTexture, 10, false);

	CMaterial* hpBarMaterial = new CMaterial(1);
	hpBarMaterial[0].SetTexture(hpBarTexture, 0);
	hpBarMaterial[0].SetShader(hpuishader);

	SetMaterial(0, hpBarMaterial);

	UIObject::UIObject();
}

HPBar::HPBar(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* rootSignature, const XMFLOAT2& position, const float& width, const float& height)
{
	CreateShaderVariables(device, commandList);

	CTexture* hpBarTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	hpBarTexture->LoadTextureFromFile(device, commandList, L"Model/Textures/Chair.dds", 1);

	HPUIShader* hpuishader = new HPUIShader();
	hpuishader->CreateShader(device, commandList, rootSignature);
	hpuishader->CreateShaderVariables(device, commandList, rootSignature);

	CScene::CreateShaderResourceViews(device, hpBarTexture, 16, false);

	CMaterial* hpBarMaterial = new CMaterial(1);
	hpBarMaterial[0].SetTexture(hpBarTexture, 0);
	hpBarMaterial[0].SetShader(hpuishader);

	SetMaterial(0, hpBarMaterial);

	UIObject::UIObject(position, width, height);
}

HPBar::~HPBar()
{
	UIObject::~UIObject();
}

unsigned int& HPBar::GetMaxHP()
{
	return maxHp;
}

unsigned int& HPBar::GetCurHP()
{
	return curHp;
}

void HPBar::SetMaxHP(const unsigned int& maxHp)
{
	this->maxHp = maxHp;
}

void HPBar::SetCurHP(const unsigned int& curHp)
{
	this->curHp = curHp;
}

void HPBar::Update()
{
	UIObject::Update();
}

void HPBar::Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* descriptorHeap, ID3D12PipelineState* uiPipelineState, D3D12_GPU_DESCRIPTOR_HANDLE srvHandle)
{
	UIObject::Render(commandList, descriptorHeap, uiPipelineState, srvHandle);
}
