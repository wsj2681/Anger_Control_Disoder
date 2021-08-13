#include "stdafx.h"
#include "PlaneObject.h"
#include "Material.h"
#include "Texture.h"
#include "TexturedRectMesh.h"
#include "Scene.h"
#include "TexturedShader.h"

PlaneObject::PlaneObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath, bool isDDS)
	:Object(1)
{
	Mesh* pMesh = new TexturedRectMesh(pd3dDevice, pd3dCommandList, 10, 10, 0);
	SetMesh(pMesh);


	Shader* pShader = new TexturedShader;
	pShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

	Texture* pTexture = new Texture;
	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filePath, 0, isDDS);
	Scene::CreateShaderResourceViews(pd3dDevice, pTexture, 15, false);

	Material* pMaterial = new Material(1);
	pMaterial->SetTexture(pTexture);

	pMaterial->SetShader(pShader);
	SetMaterial(0, pMaterial);
}

PlaneObject::~PlaneObject()
{
}

void PlaneObject::SetLookAt(XMFLOAT3& target)
{
	XMFLOAT3 xmf3Up = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 xmf3Position(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43);
	XMFLOAT3 xmf3Look = Vector3::Subtract(target, xmf3Position);
	XMFLOAT3 xmf3Right = Vector3::CrossProduct(xmf3Up, xmf3Look);

	m_xmf4x4World._11 = xmf3Right.x, m_xmf4x4World._12 = xmf3Right.y, m_xmf4x4World._13 = xmf3Right.z;
	m_xmf4x4World._21 = xmf3Up.x, m_xmf4x4World._22 = xmf3Up.y, m_xmf4x4World._23 = xmf3Up.z;
	m_xmf4x4World._31 = xmf3Look.x, m_xmf4x4World._32 = xmf3Look.y, m_xmf4x4World._33 = xmf3Look.z;
}

void PlaneObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	if (this->isActive)
	{
		XMFLOAT3 targetPosition = pCamera->GetPosition();
		SetLookAt(targetPosition);
		Object::Render(pd3dCommandList, pCamera);
	}
}
