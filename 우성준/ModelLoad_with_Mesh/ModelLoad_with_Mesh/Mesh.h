//------------------------------------------------------- ----------------------
// File: Mesh.h
//-----------------------------------------------------------------------------

#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CMeshFromFbx
{
public:
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
