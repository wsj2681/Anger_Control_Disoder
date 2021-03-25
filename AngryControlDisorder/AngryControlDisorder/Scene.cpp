#include "framework.h"
#include "Scene.h"

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
}

D3D12_GPU_DESCRIPTOR_HANDLE Scene::CreateConstantBufferViews(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nConstantBufferViews, ID3D12Resource* pd3dConstantBuffers, UINT nStride)
{
	return D3D12_GPU_DESCRIPTOR_HANDLE();
}

D3D12_GPU_DESCRIPTOR_HANDLE Scene::CreateShaderResourceViews(ID3D12Device* pd3dDevice, Texture* pTexture, UINT nRootParameter, bool bAutoIncrement)
{
	return D3D12_GPU_DESCRIPTOR_HANDLE();
}

D3D12_CPU_DESCRIPTOR_HANDLE Scene::GetCPUCbvDescriptorStartHandle()
{
	return D3D12_CPU_DESCRIPTOR_HANDLE();
}

D3D12_GPU_DESCRIPTOR_HANDLE Scene::GetGPUCbvDescriptorStartHandle()
{
	return D3D12_GPU_DESCRIPTOR_HANDLE();
}

D3D12_CPU_DESCRIPTOR_HANDLE Scene::GetCPUSrvDescriptorStartHandle()
{
	return D3D12_CPU_DESCRIPTOR_HANDLE();
}

D3D12_GPU_DESCRIPTOR_HANDLE Scene::GetGPUSrvDescriptorStartHandle()
{
	return D3D12_GPU_DESCRIPTOR_HANDLE();
}

D3D12_CPU_DESCRIPTOR_HANDLE Scene::GetCPUCbvDescriptorNextHandle()
{
	return D3D12_CPU_DESCRIPTOR_HANDLE();
}

D3D12_GPU_DESCRIPTOR_HANDLE Scene::GetGPUCbvDescriptorNextHandle()
{
	return D3D12_GPU_DESCRIPTOR_HANDLE();
}

D3D12_CPU_DESCRIPTOR_HANDLE Scene::GetCPUSrvDescriptorNextHandle()
{
	return D3D12_CPU_DESCRIPTOR_HANDLE();
}

D3D12_GPU_DESCRIPTOR_HANDLE Scene::GetGPUSrvDescriptorNextHandle()
{
	return D3D12_GPU_DESCRIPTOR_HANDLE();
}
