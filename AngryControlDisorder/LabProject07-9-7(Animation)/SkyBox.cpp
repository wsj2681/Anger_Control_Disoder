#include "stdafx.h"
#include "SkyBox.h"
#include "Texture.h"
#include "Material.h"
#include "Scene.h"
#include "SkyBoxMesh.h"
#include "SkyBoxShader.h"

SkyBox::SkyBox(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature) : Object(1)
{
	CSkyBoxMesh* pSkyBoxMesh = new CSkyBoxMesh(pd3dDevice, pd3dCommandList, 20.0f, 20.0f, 2.0f);
	SetMesh(pSkyBoxMesh);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	Texture* pSkyBoxTexture = new Texture(1, RESOURCE_TEXTURE_CUBE, 0);
	pSkyBoxTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"SkyBox/SkyBox_0.dds", 0);

	SkyBoxShader* pSkyBoxShader = new SkyBoxShader();
	pSkyBoxShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	pSkyBoxShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	Scene::CreateShaderResourceViews(pd3dDevice, pSkyBoxTexture, 10, false);

	Material* pSkyBoxMaterial = new Material(1);
	pSkyBoxMaterial->SetTexture(pSkyBoxTexture);
	pSkyBoxMaterial->SetShader(pSkyBoxShader);

	SetMaterial(0, pSkyBoxMaterial);
}

SkyBox::~SkyBox()
{
}

void SkyBox::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	XMFLOAT3 xmf3CameraPos = pCamera->GetPosition();
	SetPosition(xmf3CameraPos.x, xmf3CameraPos.y, xmf3CameraPos.z);

	Object::Render(pd3dCommandList, pCamera);
}