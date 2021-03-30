#include "framework.h"
#include "Scene.h"
#include "Texture.h"
#include "Material.h"

ID3D12DescriptorHeap* Scene::cbvSrvDescriptorHeap = nullptr;

D3D12_CPU_DESCRIPTOR_HANDLE	Scene::cbvCPUDescriptorStartHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	Scene::cbvGPUDescriptorStartHandle;
D3D12_CPU_DESCRIPTOR_HANDLE	Scene::srvCPUDescriptorStartHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	Scene::srvGPUDescriptorStartHandle;

D3D12_CPU_DESCRIPTOR_HANDLE	Scene::cbvCPUDescriptorNextHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	Scene::cbvGPUDescriptorNextHandle;
D3D12_CPU_DESCRIPTOR_HANDLE	Scene::srvCPUDescriptorNextHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	Scene::srvGPUDescriptorNextHandle;

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::BuildObjects(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
}

void Scene::BuildDefaultLightsAndMaterials()
{
}

void Scene::ReleaseObjects()
{
}

ID3D12RootSignature* Scene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
	return nullptr;
}

ID3D12RootSignature* Scene::GetGraphicsRootSignature()
{
	return nullptr;
}

void Scene::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void Scene::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void Scene::ReleaseShaderVariables()
{
}

bool Scene::ProcessInput(UCHAR* pKeysBuffer)
{
	return false;
}

void Scene::AnimateObjects(float fTimeElapsed)
{
}

void Scene::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
}

void Scene::ReleaseUploadBuffers()
{
}

bool Scene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool Scene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void Scene::CreateCbvSrvDescriptorHeaps(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews)
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = nConstantBufferViews + nShaderResourceViews; //CBVs + SRVs 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	HR(pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&cbvSrvDescriptorHeap));

	cbvCPUDescriptorNextHandle = cbvCPUDescriptorStartHandle = cbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	cbvGPUDescriptorNextHandle = cbvGPUDescriptorStartHandle = cbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	srvCPUDescriptorNextHandle.ptr = srvCPUDescriptorStartHandle.ptr = cbvCPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
	srvGPUDescriptorNextHandle.ptr = srvGPUDescriptorStartHandle.ptr = cbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
}

D3D12_GPU_DESCRIPTOR_HANDLE Scene::CreateConstantBufferViews(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nConstantBufferViews, ID3D12Resource* pd3dConstantBuffers, UINT nStride)
{
	D3D12_GPU_DESCRIPTOR_HANDLE d3dCbvGPUDescriptorHandle = cbvGPUDescriptorNextHandle;
	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = pd3dConstantBuffers->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC d3dCBVDesc;
	d3dCBVDesc.SizeInBytes = nStride;
	for (int j = 0; j < nConstantBufferViews; j++)
	{
		d3dCBVDesc.BufferLocation = d3dGpuVirtualAddress + (nStride * j);
		cbvCPUDescriptorNextHandle.ptr = cbvCPUDescriptorNextHandle.ptr + ::gnCbvSrvDescriptorIncrementSize;
		pd3dDevice->CreateConstantBufferView(&d3dCBVDesc, cbvCPUDescriptorNextHandle);
		cbvGPUDescriptorNextHandle.ptr = cbvGPUDescriptorNextHandle.ptr + ::gnCbvSrvDescriptorIncrementSize;
	}
	return d3dCbvGPUDescriptorHandle;
}

D3D12_GPU_DESCRIPTOR_HANDLE Scene::CreateShaderResourceViews(ID3D12Device* pd3dDevice, Texture* pTexture, UINT nRootParameter, bool bAutoIncrement)
{
	D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGPUDescriptorHandle = srvGPUDescriptorNextHandle;
	if (pTexture)
	{
		int nTextures = pTexture->GetTextureCount();
		int nTextureType = pTexture->GetTextureType();
		for (int i = 0; i < nTextures; i++)
		{
			ID3D12Resource* pShaderResource = pTexture->GetTexture(i);
			D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();
			D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = ::GetShaderResourceViewDesc(d3dResourceDesc, nTextureType);
			pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, srvCPUDescriptorNextHandle);
			srvCPUDescriptorNextHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;

			pTexture->SetRootArgument(i, (bAutoIncrement) ? (nRootParameter + i) : nRootParameter, srvGPUDescriptorNextHandle);
			srvGPUDescriptorNextHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;
		}
	}
	return(d3dSrvGPUDescriptorHandle);
}

D3D12_CPU_DESCRIPTOR_HANDLE Scene::GetCPUCbvDescriptorStartHandle()
{
	return this->cbvCPUDescriptorStartHandle;
}

D3D12_GPU_DESCRIPTOR_HANDLE Scene::GetGPUCbvDescriptorStartHandle()
{
	return this->cbvGPUDescriptorStartHandle;
}

D3D12_CPU_DESCRIPTOR_HANDLE Scene::GetCPUSrvDescriptorStartHandle()
{
	return this->srvCPUDescriptorStartHandle;
}

D3D12_GPU_DESCRIPTOR_HANDLE Scene::GetGPUSrvDescriptorStartHandle()
{
	return this->srvGPUDescriptorStartHandle;
}

D3D12_CPU_DESCRIPTOR_HANDLE Scene::GetCPUCbvDescriptorNextHandle()
{
	return this->cbvCPUDescriptorNextHandle;
}

D3D12_GPU_DESCRIPTOR_HANDLE Scene::GetGPUCbvDescriptorNextHandle()
{
	return this->cbvGPUDescriptorNextHandle;
}

D3D12_CPU_DESCRIPTOR_HANDLE Scene::GetCPUSrvDescriptorNextHandle()
{
	return this->srvCPUDescriptorNextHandle;
}

D3D12_GPU_DESCRIPTOR_HANDLE Scene::GetGPUSrvDescriptorNextHandle()
{
	return this->srvGPUDescriptorNextHandle;
}
