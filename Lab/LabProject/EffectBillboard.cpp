#include "stdafx.h"
#include "EffectBillboard.h"
#include "Material.h"
#include "Texture.h"
#include "Scene.h"

EffectBillboard::EffectBillboard(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	Texture* pTexture = new Texture;
	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UI/DDSfile/FrameAnimation.dds", 0);
	Scene::CreateShaderResourceViews(pd3dDevice, pTexture, 21, false);

	Shader* pShader = new BillboardAnimationShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, L"UI/DDSfile/FrameAnimation.dds");
	SetShader(pShader);

	Material* pMaterial = new Material(1);
	SetMaterial(0, pMaterial);

	Mesh* pMesh = new BillboardMesh(pd3dDevice, pd3dCommandList, 150, 150, 0, 0, 0, 0);
	SetMesh(pMesh);
	
	
	pMaterial->SetTexture(pTexture, 0);


}

EffectBillboard::~EffectBillboard()
{
}

void EffectBillboard::Update(float fTimeElapsed)
{
	if ((elapsedTime += fTimeElapsed) > 0.1f)
	{
		keyFrame++;
		elapsedTime = 0.f;
	}
}

void EffectBillboard::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	OnPrepareRender();
	Object::Render(pd3dCommandList, pCamera);
}
