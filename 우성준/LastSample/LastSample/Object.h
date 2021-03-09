#pragma once
#include "Shader.h"
#include "Mesh.h"

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

class Material final
{
public:
	Material() = default;
	Material(const Material&) = delete;
	Material& operator=(const Material&) = delete;
	virtual ~Material() = default;

private:
	Texture* texture = nullptr;
	Shader* shader = nullptr;
	XMFLOAT4 albedo{ 1.f, 1.f, 1.f, 1.f };
public:

	void SetAlbedo(XMFLOAT4 albedo) { this->albedo = albedo; }
	void SetShader(Shader* shader) { this->shader = shader; }
	void SetTexrue(Texture* texture) { this->texture = texture; }

	Texture* GetTextrue() { return texture; }
	Shader* GetShader() { return shader; }
	XMFLOAT4 GetAlbedo() { return albedo; }

	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
	{
		if (texture)
		{
			texture->UpdateShaderVariables(pd3dCommandList);
		}
	}
	void ReleaseShaderVariables() {}

	void ReleaseUploadBuffers() {}

};


class Object
{
public:
	Object() = default;
	Object(const Object&) = delete;
	Object& operator=(const Object&) = delete;
	virtual ~Object() = default;

protected:
	XMFLOAT4X4 world;

	ID3D12Resource* cbGameObject = nullptr;
	CB_GAMEOBJECT_INFO* cbMappedGameObject = nullptr;

	Mesh* mesh = nullptr;
	Material* material = nullptr;
	D3D12_GPU_DESCRIPTOR_HANDLE m_d3dCbvGPUDescriptorHandle;

public:

	void SetMesh(Mesh* mesh) { this->mesh = mesh; }
	void SetShader(Shader* shader) 
	{
		if (!this->material)
		{
			Material* mat = new Material();
			mat->SetShader(shader);
			SetMaterial(mat);
		}
		if (material)
		{
			material->SetShader(shader);
		}
	}
	void SetMaterial(Material* material) { this->material = material; }

	void SetCbvGPUDescriptorHandle(D3D12_GPU_DESCRIPTOR_HANDLE d3dCbvGPUDescriptorHandle) { m_d3dCbvGPUDescriptorHandle = d3dCbvGPUDescriptorHandle; }
	void SetCbvGPUDescriptorHandlePtr(UINT64 nCbvGPUDescriptorHandlePtr) { m_d3dCbvGPUDescriptorHandle.ptr = nCbvGPUDescriptorHandlePtr; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetCbvGPUDescriptorHandle() { return(m_d3dCbvGPUDescriptorHandle); }

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual void Animate(float fTimeElapsed);
	virtual void OnPrepareRender() { }
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera = NULL);

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);

};