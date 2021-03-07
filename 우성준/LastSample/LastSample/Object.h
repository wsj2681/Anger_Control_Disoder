#pragma once

struct CB_GAMEOBJECT_INFO
{
	XMFLOAT4X4 world;
};

// Texture Type
#define RESOURCE_TEXTURE2D			0x01
#define RESOURCE_TEXTURE2D_ARRAY	0x02	//[]
#define RESOURCE_TEXTURE2DARRAY		0x03
#define RESOURCE_TEXTURE_CUBE		0x04
#define RESOURCE_BUFFER				0x05

class Texture final
{
public:
	Texture(int textureResources, UINT resourceType, int samplers, int rootParameters);
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	virtual ~Texture() = default;

private:

	UINT textureType;

	int textureCount = 0;
	ID3D12Resource** textures = nullptr;
	ID3D12Resource** textureUploadBuffer = nullptr;
	UINT* resourceType = nullptr;

	DXGI_FORMAT* bufferFormats = nullptr;
	int* bufferElements = nullptr;

	int rootParameterCount = 0;
	UINT* rootParameterIndices = nullptr;
	D3D12_GPU_DESCRIPTOR_HANDLE* srvGPUDescHandles = nullptr;

	int samplers = 0;
	D3D12_GPU_DESCRIPTOR_HANDLE* samplerGPUDescHandles = nullptr;

public:

	void SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle);

	void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, int nParameterIndex, int nTextureIndex);
	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	void ReleaseShaderVariables();

	void LoadTextureFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nResourceType, UINT nIndex);

	void SetRootParameterIndex(int nIndex, UINT nRootParameterIndex);
	void SetGpuDescriptorHandle(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle);

	D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(int nIndex);

	void ReleaseUploadBuffers();

	int GetRootParameters() { return rootParameterCount; }
	int GetTextures() { return textureCount; }
	ID3D12Resource* GetResource(int nIndex) { return textures[nIndex]; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuDescriptorHandle(int nIndex) { return srvGPUDescHandles[nIndex]; }
	int GetRootParameter(int nIndex) { return rootParameterIndices[nIndex]; }

	UINT GetTextureType() { return textureType; }
	UINT GetTextureType(int nIndex) { return resourceType[nIndex]; }


};