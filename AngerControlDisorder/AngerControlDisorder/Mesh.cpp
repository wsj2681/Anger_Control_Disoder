#include "stdafx.h"
#include "Mesh.h"
#include "Object.h"

Mesh::Mesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
}

Mesh::~Mesh()
{
	SAFE_RELEASE(m_pd3dPositionBuffer);

	if (m_nSubMeshes > 0)
	{
		for (int i = 0; i < m_nSubMeshes; i++)
		{
			SAFE_RELEASE(m_ppd3dSubSetIndexBuffers[i]);
			SAFE_DELETEARR(m_ppnSubSetIndices[i]);
		}
		SAFE_DELETEARR(m_ppd3dSubSetIndexBuffers);
		SAFE_DELETEARR(m_pd3dSubSetIndexBufferViews);

		SAFE_DELETEARR(m_pnSubSetIndices);
		SAFE_DELETEARR(m_ppnSubSetIndices);
	}

	SAFE_DELETEARR(m_pxmf3Positions);
}

void Mesh::ReleaseUploadBuffers()
{
	SAFE_RELEASE(m_pd3dPositionUploadBuffer);

	if ((m_nSubMeshes > 0) && m_ppd3dSubSetIndexUploadBuffers)
	{
		for (int i = 0; i < m_nSubMeshes; i++) SAFE_RELEASE(m_ppd3dSubSetIndexUploadBuffers[i]);
		SAFE_DELETEARR(m_ppd3dSubSetIndexUploadBuffers);
	}
}

void Mesh::OnPreRender(ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, &m_d3dPositionBufferView);
}

void Mesh::Render(ID3D12GraphicsCommandList *pd3dCommandList, int nSubSet)
{
	UpdateShaderVariables(pd3dCommandList);

	OnPreRender(pd3dCommandList, nullptr);

	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	if((m_nSubMeshes > 0) && (nSubSet < m_nSubMeshes))
	{
		pd3dCommandList->IASetIndexBuffer(&(m_pd3dSubSetIndexBufferViews[nSubSet]));
		pd3dCommandList->DrawIndexedInstanced(m_pnSubSetIndices[nSubSet], 1, 0, 0, 0);
	}
	else
	{
		pd3dCommandList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
	}
}

void Mesh::OnPostRender(ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
}

bool Mesh::isIntersect(BoundingOrientedBox& otherBox)
{
	if (obb.Intersects(otherBox))
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}
