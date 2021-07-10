#include "stdafx.h"
#include "CollideCubeObject.h"
#include "CollideCubeMesh.h"
#include "Material.h"
#include "DiffusedShader.h"

CollideCubeObject::CollideCubeObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	CollideCubeMesh* pSkyBoxMesh = new CollideCubeMesh(pd3dDevice, pd3dCommandList, 10.0f, 10.0f, 10.0f);
	SetMesh(pSkyBoxMesh);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	DiffusedShader* pSkyBoxShader = new DiffusedShader();
	pSkyBoxShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	pSkyBoxShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	Material* pSkyBoxMaterial = new Material(0);
	pSkyBoxMaterial->SetShader(pSkyBoxShader);

	//SetMaterial(0, pSkyBoxMaterial);
	SetShader(pSkyBoxShader);
}
