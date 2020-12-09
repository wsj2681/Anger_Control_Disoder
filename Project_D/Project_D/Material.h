//#pragma once
//
//class Texture;
//class Shader;
//
//struct CB_GAMEOBJECT_INFO
//{
//	XMFLOAT4X4						m_xmf4x4World;
//	UINT							m_nMaterial;
//};
//
//struct MATERIAL
//{
//	XMFLOAT4						m_xmf4Ambient;
//	XMFLOAT4						m_xmf4Diffuse;
//	XMFLOAT4						m_xmf4Specular;
//	XMFLOAT4						m_xmf4Emissive;
//};
//
//class Material
//{
//public:
//	Material() = default;
//	virtual ~Material();
//private:
//	int nRefers{ 0 };
//	XMFLOAT4 albedo{ 1.f, 1.f, 1.f, 1.f };
//	unsigned int nReflection{ 0 };
//	XMFLOAT4X4* reflection{ nullptr };
//	Texture* texture{ nullptr };
//	Shader* shader{ nullptr };
//public:
//	void SetAlbedo(const XMFLOAT4& albedo);
//	void SetReflection(const unsigned int nReflection);
//	void setReflection(XMFLOAT4X4* reflection);
//	void SetTexture(Texture* texture);
//	void SetShader(Shader* shader);
//
//	void AddRef();
//	void Release();
//};