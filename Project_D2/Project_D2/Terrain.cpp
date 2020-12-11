#include "stdafx.h"
#include "Shader.h"
#include "Texture.h"
#include "Terrain.h"
#include "Material.h"
#include "Mesh.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
HeightMapTerrain::HeightMapTerrain(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color)
	: CGameObject(1)
{
	m_nWidth = nWidth;
	m_nLength = nLength;

	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;

	m_xmf3Scale = xmf3Scale;

	m_pHeightMapImage = new HeightMapImage(pFileName, nWidth, nLength, xmf3Scale);

	long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	long czBlocks = (m_nLength - 1) / czQuadsPerBlock;

	m_nMeshes = cxBlocks * czBlocks;
	m_ppMeshes = new Mesh * [m_nMeshes];
	for (int i = 0; i < m_nMeshes; i++)	
		m_ppMeshes[i] = NULL;

	HeightMapGridMesh* pHeightMapGridMesh = NULL;
	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);
			pHeightMapGridMesh = new HeightMapGridMesh(pd3dDevice, pd3dCommandList, xStart, zStart, nBlockWidth, nBlockLength, xmf3Scale, xmf4Color, m_pHeightMapImage);
			SetMesh(x + (z * cxBlocks), pHeightMapGridMesh);
		}
	}

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	CTexture* pTerrainTexture = new CTexture(3, RESOURCE_TEXTURE2D, 0, 1);

	pTerrainTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Base_Texture.dds", RESOURCE_TEXTURE2D, 0);
	pTerrainTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Detail_Texture_7.dds", RESOURCE_TEXTURE2D, 1);
	pTerrainTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/HeightMap(Alpha).dds", RESOURCE_TEXTURE2D, 2);

	UINT ncbElementBytes = ((sizeof(CB_TERRAIN_OBJECT_INFO) + 255) & ~255); //256의 배수

	CTerrainShader* pTerrainShader = new CTerrainShader();
	pTerrainShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	pTerrainShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pTerrainShader->CreateCbvSrvDescriptorHeaps(pd3dDevice, 1, 3);
	//pTerrainShader->CreateConstantBufferViews(pd3dDevice, 1, m_pd3dcbGameObject, ncbElementBytes);
	//pTerrainTexture->SetRootParameters(1);
	pTerrainShader->CreateShaderResourceViews(pd3dDevice, pTerrainTexture, 0, 5);

	CMaterial* pTerrainMaterial = new CMaterial();
	pTerrainMaterial->SetTexture(pTerrainTexture);

	SetMaterial(0, pTerrainMaterial);

	// SetCbvGPUDescriptorHandle(pTerrainShader->GetGPUCbvDescriptorStartHandle());

	SetShader(pTerrainShader);
}

HeightMapTerrain::~HeightMapTerrain(void)
{
	if (m_pHeightMapImage) delete m_pHeightMapImage;
}

void HeightMapTerrain::SetMesh(int nIndex, Mesh* pMesh)
{
	if (m_ppMeshes)
	{
		if (m_ppMeshes[nIndex]) m_ppMeshes[nIndex]->Release();
		m_ppMeshes[nIndex] = pMesh;
		if (pMesh) pMesh->AddRef();
	}
}

void HeightMapTerrain::ReleaseShaderVariables()
{
	//if (m_pd3dcbGameObject)
	//{
	//	m_pd3dcbGameObject->Unmap(0, NULL);
	//	m_pd3dcbGameObject->Release();
	//}

	for (int i = 0; i < m_nMaterials; ++i)
	{
		m_ppMaterials[i]->ReleaseShaderVariables();
	}
}

void HeightMapTerrain::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	OnPrepareRender();

	UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);

	if (m_nMaterials > 0)
	{
		for (int i = 0; i < m_nMaterials; i++)
		{
			if (m_ppMaterials[i]->m_pShader)
			{
				m_ppMaterials[i]->m_pShader->Render(pd3dCommandList, pCamera);
				m_ppMaterials[i]->m_pShader->UpdateShaderVariables(pd3dCommandList);

				UpdateShaderVariables(pd3dCommandList);
			}
			if (m_ppMaterials[i]->m_pTexture)
			{
				m_ppMaterials[i]->m_pTexture->UpdateShaderVariables(pd3dCommandList);
			}
		}
	}

	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i]) m_ppMeshes[i]->Render(pd3dCommandList);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
TerrainWater::TerrainWater(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, float fWidth, float fLength)
	: CGameObject(1)
{
	m_xmf4x4WaterAnimation = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	TexturedRectMesh* pWaterMesh = new TexturedRectMesh(pd3dDevice, pd3dCommandList, fWidth, 0.0f, fLength, 0.0f, 0.0f, 0.0f);

	m_nMeshes = 1;
	m_ppMeshes = new Mesh * [m_nMeshes];
	for (int i = 0; i < m_nMeshes; i++)	m_ppMeshes[i] = NULL;
	SetMesh(0, pWaterMesh);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	CTexture* pWaterTexture = new CTexture(3, RESOURCE_TEXTURE2D, 0, 1);

	pWaterTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Water_Base_Texture_0.dds", RESOURCE_TEXTURE2D, 0);
	pWaterTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Water_Detail_Texture_0.dds", RESOURCE_TEXTURE2D, 1);
	pWaterTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Lava(Diffuse).dds", RESOURCE_TEXTURE2D, 2);
	//	pWaterTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Image/Water_Texture_Alpha.dds", RESOURCE_TEXTURE2D, 2);

	UINT ncbElementBytes = ((sizeof(CB_TERRAIN_OBJECT_INFO) + 255) & ~255); //256의 배수

	CTerrainWaterShader* pWaterShader = new CTerrainWaterShader();
	pWaterShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	pWaterShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pWaterShader->CreateCbvSrvDescriptorHeaps(pd3dDevice, 1, 3);

	//pWaterTexture->SetRootParameters(1);
	pWaterShader->CreateShaderResourceViews(pd3dDevice, pWaterTexture, 0, 6); 

	CMaterial* pWaterMaterial = new CMaterial();
	pWaterMaterial->SetTexture(pWaterTexture);

	SetMaterial(0, pWaterMaterial);

	SetShader(pWaterShader);
}

TerrainWater::~TerrainWater()
{
}

void TerrainWater::SetMesh(int nIndex, Mesh* pMesh)
{
	if (m_ppMeshes)
	{
		if (m_ppMeshes[nIndex]) m_ppMeshes[nIndex]->Release();
		m_ppMeshes[nIndex] = pMesh;
		if (pMesh) pMesh->AddRef();
	}
}

void TerrainWater::ReleaseShaderVariables()
{

	for (int i = 0; i < m_nMaterials; ++i)
	{
		m_ppMaterials[i]->ReleaseShaderVariables();
	}
}

void TerrainWater::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	CGameObject::UpdateShaderVariables(pd3dCommandList);
}
void TerrainWater::Animate(float fTimeElapsed)
{
}

void TerrainWater::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	OnPrepareRender();

	UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);

	if (m_nMaterials > 0)
	{
		for (int i = 0; i < m_nMaterials; i++)
		{
			if (m_ppMaterials[i]->m_pShader)
			{
				m_ppMaterials[i]->m_pShader->Render(pd3dCommandList, pCamera);
				m_ppMaterials[i]->m_pShader->UpdateShaderVariables(pd3dCommandList);

				UpdateShaderVariables(pd3dCommandList);
			}
			if (m_ppMaterials[i]->m_pTexture)
			{
				m_ppMaterials[i]->m_pTexture->UpdateShaderVariables(pd3dCommandList);
			}
		}
	}

	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i]) m_ppMeshes[i]->Render(pd3dCommandList );
		}
	}
}
