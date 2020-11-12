#pragma once
#include "Shader.h"
#include "Texture.h"

struct Phong
{
	XMFLOAT4 AlbedoColor{ 1.f, 1.f, 1.f, 1.f };
	XMFLOAT4 EmissiveColor{ 0.f, 0.f, 0.f, 1.f };
	XMFLOAT4 SpecularColor{ 0.f, 0.f, 0.f, 1.f };
	XMFLOAT4 AmbientColor{ 0.f, 0.f, 0.f, 1.f };
};

class Material
{
public:
	Material() = default;
	Material(const Material&) = delete;
	Material& operator=(const Material&) = delete;
	~Material() = default;

private:
	int references = 0;

	Phong phong;

	Shader* shader = nullptr;
	UINT Type = 0x00;

	Texture* texture = nullptr;

public:/* Material을 구성하는 변수 입니다. */

	float Glossiness = 0.f;
	float Smoothness = 0.f;
	float SpecularHightlight = 0.f;
	float Metallic = 0.f;
	float GlossyReflection = 0.f;

public:
	void AddReference();
	void Release();

	void SetShader(Shader* shader);

	void MaterialType(UINT Type);
	const UINT& GetMaterialType() const;

	void SetTexture(Texture* texture);


	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* CommandList);
	virtual void ReleaseShaderVariables();

	virtual void ReleaseUploadBUffers();
};

