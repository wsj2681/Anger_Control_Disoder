#pragma once
#include "framework.h"

class CVertex{
protected:
	//정점의 위치 벡터이다(모든 정점은 최소한 위치 벡터를 가져야 한다).
	XMFLOAT3 m_xmf3Position{ (XMFLOAT3(0.f, 0.f, 0.f)) };

public:	
	CVertex() {}
	CVertex(XMFLOAT3 xmf3Position) { m_xmf3Position = xmf3Position; }
	~CVertex() { }
};

class CDiffusedVertex : public CVertex {
protected:
	//정점의 색상이다.
	XMFLOAT4 m_xmf4Diffuse{};
public:
	CDiffusedVertex() {
		m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	CDiffusedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse) {
		m_xmf3Position = XMFLOAT3(x, y, z);
		m_xmf4Diffuse = xmf4Diffuse;
	}
	CDiffusedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse) {
		m_xmf3Position = xmf3Position;
		m_xmf4Diffuse = xmf4Diffuse;
	}
	~CDiffusedVertex() { }
};

class CMesh{
	int m_nReferences = 0;

protected:
	ID3D12Resource* m_pd3dVertexBuffer = NULL;
	ID3D12Resource* m_pd3dVertexUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW m_d3dVertexBufferView;
	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT m_nSlot = 0;
	UINT m_nVertices = 0;
	UINT m_nStride = 0;
	UINT m_nOffset = 0;

	//인덱스 버퍼
	ID3D12Resource* m_pd3dIndexBuffer		{nullptr};
	ID3D12Resource* m_pd3dIndexUploadBuffer	{nullptr};

	D3D12_INDEX_BUFFER_VIEW m_d3dIndexBufferView;

	UINT m_nIndices		{ 0 };
	UINT m_nStartIndex	{ 0 };
	int m_nBaseVertex	{ 0 };

public:
	CMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~CMesh();

	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }
	void ReleaseUploadBuffers();

	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList);
};

class CTriangleMesh : public CMesh{
public:
	CTriangleMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~CTriangleMesh() { }
};

class CCubeMeshDiffused : public CMesh {
public:
	//직육면체의 가로, 세로, 깊이의 길이를 지정하여 직육면체 메쉬를 생성한다.
	CCubeMeshDiffused(
		ID3D12Device* pd3dDevice,
		ID3D12GraphicsCommandList* pd3dCommandList,
		float fWidth = 2.0f,
		float fHeight = 2.0f,
		float fDepth = 2.0f);

	virtual ~CCubeMeshDiffused();
};

class CAirplaneMeshDiffused : public CMesh {
public:
	CAirplaneMeshDiffused(ID3D12Device* pd3dDevice,
		ID3D12GraphicsCommandList
		* pd3dCommandList,
		float fWidth = 20.0f,
		float fHeight = 20.0f,
		float fDepth = 4.0f,
		XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f));

	virtual ~CAirplaneMeshDiffused();
};

class MapMesh : public CMesh
{
public:
	MapMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	~MapMesh() = default;


	vector<XMFLOAT3> vertices;
	vector<XMFLOAT2> uvs;
	vector<XMFLOAT3> normals;
	vector<unsigned int> vertexIndices, uvIndices, normalIndices;

	void LoadMeshFromFile();
};