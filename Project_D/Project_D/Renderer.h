#pragma once
// Set Pipeline
// temporary class
//TODO : Camera
class Camera;

//TODO : Texture
class Texture {
public:
	Texture() = default;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	~Texture() = default;
public:
	int GetTextures() { return 0; }
	UINT GetTextureType() { return 0; }
	ID3D12Resource* GetResource(int index) { return nullptr; }
	D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(int index) { return D3D12_SHADER_RESOURCE_VIEW_DESC(); }
	int GetRootParameters() { return 0; }
	
	void SetGpuDescriptorHandle(int index, D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorNextHandle) {}
	void SetRootParameterIndex(int index, int rootParameterIndex) {}
};

//TODO : Renderer?
class Renderer
{
public:
	Renderer() = default;
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer() = default;	
	
};