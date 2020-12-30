#pragma once

#include "Object.h"
#include "RawFormatImage.h"

class CTxture;
class CMaterial;
class CMesh;

struct CB_TERRAIN_OBJECT_INFO
{
	XMFLOAT4X4						m_xmf4x4World;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//


class HeightMapTerrain : public CGameObject
{
public:
	HeightMapTerrain(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color);
	virtual ~HeightMapTerrain();

private:
	HeightMapImage				* m_pHeightMapImage;

	int							m_nWidth;
	int							m_nLength;

	XMFLOAT3					m_xmf3Scale;

	Mesh						**m_ppMeshes;
	int							m_nMeshes;
	void SetMesh(int nIndex, Mesh* pMesh);

	virtual void ReleaseShaderVariables();

public:
	float GetHeight(float x, float z, bool bReverseQuad = false) { return(m_pHeightMapImage->GetHeight(x, z, bReverseQuad) * m_xmf3Scale.y); } //World
	XMFLOAT3 GetNormal(float x, float z) { return(m_pHeightMapImage->GetHeightMapNormal(int(x / m_xmf3Scale.x), int(z / m_xmf3Scale.z))); }

	int GetRawImageWidth() { return(m_pHeightMapImage->GetRawImageWidth()); }
	int GetRawImageLength() { return(m_pHeightMapImage->GetRawImageLength()); }

	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	float GetWidth() { return(m_nWidth * m_xmf3Scale.x); }
	float GetLength() { return(m_nLength * m_xmf3Scale.z); }

	void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera = NULL);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class TerrainWater : public CGameObject
{
public:
	TerrainWater(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, float nWidth, float nLength);
	virtual ~TerrainWater();

private:
	int							m_nWidth;
	int							m_nLength;

	XMFLOAT3					m_xmf3Scale;

	XMFLOAT4X4					m_xmf4x4WaterAnimation;

	Mesh** m_ppMeshes;
	int							m_nMeshes;
	void SetMesh(int nIndex, Mesh* pMesh);

	virtual void ReleaseShaderVariables();

public:
	XMFLOAT4X4					m_xmf4x4Texture;

	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual void Animate(float fTimeElapsed);

	void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera = NULL);
};