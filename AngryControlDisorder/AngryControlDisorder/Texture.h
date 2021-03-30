#pragma once

#define RESOURCE_TEXTURE2D			0x01
#define RESOURCE_TEXTURE2D_ARRAY	0x02	//[]
#define RESOURCE_TEXTURE2DARRAY		0x03
#define RESOURCE_TEXTURE_CUBE		0x04
#define RESOURCE_BUFFER				0x05

class Shader;
class StandardShader;

struct SRVROOTARGUMENTINFO
{
	int rootParameterIndex = 0;
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuDescriptorHandle;
};

class Texture final
{
public:
	Texture();
	Texture(int nTextureResources = 1, UINT nResourceType = RESOURCE_TEXTURE2D, int nSamplers = 0);
	~Texture();

private:

	int references = 0;
		 
	UINT textureType = RESOURCE_TEXTURE2D;
		 
	int textureCount = 0;
	ID3D12Resource** textures = nullptr;
	ID3D12Resource** textureUploadBuffers;

	int	samplers = 0;
	D3D12_GPU_DESCRIPTOR_HANDLE* samplerGpuDescriptorHandles = nullptr;

	SRVROOTARGUMENTINFO* rootArgumentInfos = nullptr;

public:

	void AddRef();
	void Release();

	void SetRootArgument(int nIndex, UINT nRootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dsrvGpuDescriptorHandle);
	void SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle);

	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, int nIndex);
	void ReleaseShaderVariables();

	void LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nIndex, bool bIsDDSFile = true);

	int GetTextureCount();
	ID3D12Resource* GetTexture(int nIndex);
	UINT GetTextureType();

	void ReleaseUploadBuffers();

};

