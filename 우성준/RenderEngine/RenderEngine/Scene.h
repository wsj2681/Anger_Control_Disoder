#pragma once

class Camera;

class Scene
{
public:
	
	Scene() = default;
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	virtual ~Scene() = default;

protected:

	ID3D12RootSignature* graphicsRootSignature = nullptr;

public:

	virtual void BuildObjects(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, FbxManager* pfbxSdkManager, FbxScene* pfbxScene);
	virtual void ReleaseObjects();
	
	virtual void CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* commandList);
	virtual void ReleaseShaderVariables();

	virtual ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* device);
	virtual ID3D12RootSignature* GetGraphicsRootSignature();

	virtual void AnimateObjects(float eTime);
	virtual void Render(ID3D12GraphicsCommandList* commandList, Camera* camera = nullptr);

};