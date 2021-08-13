#include "stdafx.h"
#include "CubeObject.h"
#include "DiffusedShader.h"
#include "Material.h"
#include "CubeMesh.h"
CubeObject::CubeObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	CubeMesh* pSkyBoxMesh = new CubeMesh(pd3dDevice, pd3dCommandList, 20.0f, 20.0f, 20.0f);
	SetMesh(pSkyBoxMesh);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	DiffusedShader* pSkyBoxShader = new DiffusedShader();
	pSkyBoxShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	pSkyBoxShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	Material* pSkyBoxMaterial = new Material(0);
	pSkyBoxMaterial->SetShader(pSkyBoxShader);

	SetShader(pSkyBoxShader);
}

CubeObject::CubeObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, const float& x, const float& y, const float& z)
{
	CubeMesh* pSkyBoxMesh = new CubeMesh(pd3dDevice, pd3dCommandList, x, y, z);
	SetMesh(pSkyBoxMesh);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	DiffusedShader* pSkyBoxShader = new DiffusedShader();
	pSkyBoxShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	pSkyBoxShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	Material* pSkyBoxMaterial = new Material(0);
	pSkyBoxMaterial->SetShader(pSkyBoxShader);

	SetShader(pSkyBoxShader);
}

CubeObject::CubeObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, const float& x, const float& y, const float& z, const char* name)
{
	CubeMesh* pSkyBoxMesh = new CubeMesh(pd3dDevice, pd3dCommandList, name, x, y, z);
	SetMesh(pSkyBoxMesh);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	DiffusedShader* pSkyBoxShader = new DiffusedShader();
	pSkyBoxShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	pSkyBoxShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	Material* pSkyBoxMaterial = new Material(0);
	pSkyBoxMaterial->SetShader(pSkyBoxShader);

	SetShader(pSkyBoxShader);
}

CubeObject::~CubeObject()
{

}

void CubeObject::Update(const float& fElapsedTime, Object* bone)
{
	this->m_xmf4x4World = bone->m_xmf4x4World;
	m_pMesh->Update(bone);
}