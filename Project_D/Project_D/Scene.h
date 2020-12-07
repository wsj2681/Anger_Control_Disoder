#pragma once

class Camera;

class Scene
{
public:

	Scene() = default;
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	virtual ~Scene() = default;

private:

protected:
	
	ID3D12RootSignature* graphicsRootSignature = nullptr;
	
public:

	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseObjects();

	ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
	ID3D12RootSignature* GetGraphicsRootSignature();

	void SetGraphicsRootSignature(ID3D12GraphicsCommandList* pd3dCommandList);

	void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	void ReleaseShaderVariables();
	
	void Animate(float etime);

	void OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	void OnPreRender(ID3D12Device* pd3dDevice, ID3D12CommandQueue* pd3dCommandQueue, ID3D12Fence* pd3dFence, HANDLE hFenceEvent);

	void Render(ID3D12GraphicsCommandList *pd3dCommandList, Camera *pCamera);
	
	void ReleaseUploadBuffer();
};