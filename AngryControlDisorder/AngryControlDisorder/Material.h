#pragma once

#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40

class Shader;
class Texture;
class Object;


class Material final
{
public:

	Material();
	Material(int textures);
	~Material();

public:

	int references = 0;

	Shader* shader = nullptr;

	XMFLOAT4 albedoColor{ 1.f, 1.f, 1.f, 1.f };
	XMFLOAT4 emissiveColor{ 0.f, 0.f, 0.f, 1.f };
	XMFLOAT4 specularColor{ 0.f, 0.f, 0.f, 1.f };
	XMFLOAT4 ambientColor{ 0.f, 0.f, 0.f, 1.f };

	UINT type = 0x00;

	float glossiness = 0.0f;
	float smoothness = 0.0f;
	float specularHighlight = 0.0f;
	float metallic = 0.0f;
	float glossyReflection = 0.0f;

	int textureCount = 0;
	_TCHAR (*textureNames)[64] = nullptr;
	//0:Albedo, 1:Specular, 2:Metallic, 3:Normal, 4:Emission, 5:DetailAlbedo, 6:DetailNormal
	Texture** textures = nullptr;

	static Shader* standardShader;
	static Shader* skinnedAnimationShader;

public:

	void AddRef();
	void Release();

	void SetShader(Shader* shader);
	Shader* GetShader();
	_TCHAR* GetTextureNames(int nIndex);
	Texture* GetTextures(int nIndex);
	int GetTextureCount();
	void SetMaterialType(UINT type);
	void SetTexture(Texture* texture, UINT textureCount = 0);

	void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList);
	void ReleaseUploadBuffers();

	void SetStandardShader();
	void SetSkinnedAnimationShader();

	void LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nType, UINT nRootParameter, _TCHAR* pwstrTextureName, Texture** ppTexture, Object* pParent, FILE* pInFile, Shader* pShader);
	
	static void PrepareShaders(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
};

