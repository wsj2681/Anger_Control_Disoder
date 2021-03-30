#pragma once

#include "light.h"

class Camera;
class Texture;

class Scene
{
public:
	Scene();
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	virtual ~Scene();

private:



protected:
	ID3D12RootSignature* graphicsRootSignature = nullptr;

	static ID3D12DescriptorHeap* cbvSrvDescriptorHeap;

	static D3D12_CPU_DESCRIPTOR_HANDLE	cbvCPUDescriptorStartHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	cbvGPUDescriptorStartHandle;
	static D3D12_CPU_DESCRIPTOR_HANDLE	srvCPUDescriptorStartHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	srvGPUDescriptorStartHandle;

	static D3D12_CPU_DESCRIPTOR_HANDLE	cbvCPUDescriptorNextHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	cbvGPUDescriptorNextHandle;
	static D3D12_CPU_DESCRIPTOR_HANDLE	srvCPUDescriptorNextHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	srvGPUDescriptorNextHandle;

public:

	void BuildObjects(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	void BuildDefaultLightsAndMaterials();
	void ReleaseObjects();

	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	ID3D12RootSignature* GetGraphicsRootSignature();

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	bool ProcessInput(UCHAR* pKeysBuffer);
	void AnimateObjects(float fTimeElapsed);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera = nullptr);

	void ReleaseUploadBuffers();

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	static void CreateCbvSrvDescriptorHeaps(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews);

	static D3D12_GPU_DESCRIPTOR_HANDLE CreateConstantBufferViews(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nConstantBufferViews, ID3D12Resource* pd3dConstantBuffers, UINT nStride);
	static D3D12_GPU_DESCRIPTOR_HANDLE CreateShaderResourceViews(ID3D12Device* pd3dDevice, Texture* pTexture, UINT nRootParameter, bool bAutoIncrement);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUCbvDescriptorStartHandle();
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUCbvDescriptorStartHandle();
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSrvDescriptorStartHandle();
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSrvDescriptorStartHandle();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUCbvDescriptorNextHandle();
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUCbvDescriptorNextHandle();
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSrvDescriptorNextHandle();
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSrvDescriptorNextHandle();


};

