#pragma once

#define RESOURCE_TEXTURE2D			0x01
#define RESOURCE_TEXTURE2D_ARRAY	0x02	//[]
#define RESOURCE_TEXTURE2DARRAY		0x03
#define RESOURCE_TEXTURE_CUBE		0x04
#define RESOURCE_BUFFER				0x05

class Object;
class Texture;
class Material;
class Shader;

struct CB_GAMEOBJECT_INFO
{
	XMFLOAT4X4 world;
	UINT material;
};

struct MATERIAL
{
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular; //(r,g,b,a=power)
	XMFLOAT4 emissive;
};

struct SRVROOTARGUMENTINFO
{
	UINT							m_nRootParameterIndex = 0;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_d3dSrvGpuDescriptorHandle;
};

class Texture final
{
public:
	Texture(int textures = 1, UINT textureType = RESOURCE_TEXTURE2D, int samplers = 0);
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	~Texture() = default;

private:
	UINT texturType = RESOURCE_TEXTURE2D;

	int textureCount = 0;
	ID3D12Resource** textures = nullptr;

	ID3D12Resource** textureUploadBuffers = nullptr;
	
	SRVROOTARGUMENTINFO* rootArgumentInfos = nullptr;

	int samplers = 0;
	D3D12_GPU_DESCRIPTOR_HANDLE* samplerGpuDescHandles = nullptr;

public:

	void SetRootArgument(int nIndex, UINT nRootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dsrvGpuDescriptorHandle);
	void SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle);

	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, int nIndex);
	void ReleaseShaderVariables();

	void LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nIndex);
	void LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* pszFileName, UINT nIndex);

	int GetTextureCount();
	ID3D12Resource* GetTexture(int index);
	UINT GetTextureType();


};

class Material final
{
public:
	Material() = default;
	Material(int textures);
	Material(const Material&) = delete;
	Material& operator=(const Material&) = delete;
	~Material() = default;
private:
	XMFLOAT4 albedo{ 1.f, 1.f, 1.f, 1.f };
	UINT reflection = 0;
	Texture* texture = nullptr;
	Shader* shader = nullptr;

public:

	void SetAlbedo(XMFLOAT4 xmf4Albedo);
	void SetReflection(UINT nReflection);
	void SetTexture(Texture* pTexture);
	void SetShader(Shader* pShader);

	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	void ReleaseShaderVariables();

	void ReleaseUploadBuffers();
};

class Object
{
public:
	Object() = default;
	Object(const Object&) = delete;
	Object& operator=(const Object&) = delete;
	virtual ~Object() = default;

protected:

public:

};

class RingObject : public Object
{
public:
	RingObject() = default;
	RingObject(const RingObject&) = delete;
	RingObject& operator=(const RingObject&) = delete;
	virtual ~RingObject() = default;

private:

public:

};

class ParticleObject : public Object
{
public:
	ParticleObject() = default;
	ParticleObject(const ParticleObject&) = delete;
	ParticleObject& operator=(const ParticleObject&) = delete;
	virtual ~ParticleObject() = default;

private:

public:
};