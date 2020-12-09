#pragma once

class Texture {
public:
	Texture() = default;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	~Texture() = default;
private:
	int nRefers{ 0 };

	int nTextures{ 0 };
	UINT nTextureType{ 0 };
	UINT* resourceTypes{ 0 };

	ID3D12Resource** textures{ nullptr };
	ID3D12Resource** textureUploadBuffers{ nullptr };
public:
	const int& GetTextures() const { return 0; }
	const UINT& GetTextureType() const { return 0; }
	const UINT& GetTextureType(const int& index) const { return 0; }
	ID3D12Resource* GetResource(int index) { return nullptr; }
	const D3D12_SHADER_RESOURCE_VIEW_DESC& GetShaderResourceViewDesc(int index) { return D3D12_SHADER_RESOURCE_VIEW_DESC(); }
	int GetRootParameters() { return 0; }

	void SetGpuDescriptorHandle(int index, D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorNextHandle) {}
	void SetRootParameterIndex(int index, int rootParameterIndex) {}

	void AddRef() {}
	void Release() {}
};