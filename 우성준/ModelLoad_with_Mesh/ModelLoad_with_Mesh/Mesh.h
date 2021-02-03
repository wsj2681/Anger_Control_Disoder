//------------------------------------------------------- ----------------------
// File: Mesh.h
//-----------------------------------------------------------------------------

#pragma once
class Vertex
{
protected:
	XMFLOAT3 position{ 0.f, 0.f, 0.f };

public:
	Vertex() {};
	Vertex(const XMFLOAT3& position) { this->position = position; }
	~Vertex() {};
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CMeshFromFbx
{
public:
	CMeshFromFbx() = default;
	CMeshFromFbx(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nVertices, int nIndices, int *pnIndices);
	virtual ~CMeshFromFbx();

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

protected:
	D3D12_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT							m_nSlot = 0;
	UINT							m_nOffset = 0;

protected:
	int								m_nVertices = 0;

	ID3D12Resource					*m_pd3dPositionBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dPositionBufferView;

	int								m_nIndices = 0;

	ID3D12Resource					*m_pd3dIndexBuffer = NULL;
	ID3D12Resource					*m_pd3dIndexUploadBuffer = NULL;
	D3D12_INDEX_BUFFER_VIEW			m_d3dIndexBufferView;

public:
	XMFLOAT4						*m_pxmf4MappedPositions = NULL;

public:
	virtual void ReleaseUploadBuffers();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList);
};

class ParticleMesh : public CMeshFromFbx
{
public:
	ParticleMesh() = default;
	ParticleMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~ParticleMesh();

protected:
	ID3D12Resource* m_pd3dVertexBuffer = nullptr;
	ID3D12Resource* m_pd3dVertexUploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_d3dVertexBufferView;



};