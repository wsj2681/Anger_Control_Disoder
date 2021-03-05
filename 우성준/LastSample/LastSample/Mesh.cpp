#include "framework.h"
#include "Mesh.h"

Mesh::Mesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

Mesh::Mesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nVertices, XMFLOAT3* pxmf3Positions, UINT nIndices, UINT* pnIndices)
{
	vertices = nVertices;

	Vertex* pVertices = new Vertex[vertices];
	for (int i = 0; i < vertices; ++i)
	{
		pVertices[i] = Vertex(pxmf3Positions[i]);
	}

	vertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices, sizeof(Vertex) * vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &vertexUploadBuffer);

	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(Vertex);
	vertexBufferView.SizeInBytes = sizeof(Vertex) * vertices;

	if (nIndices > 0)
	{
		indices = nIndices;

		indexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pnIndices, sizeof(UINT) * indices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &indexUploadBuffer);
		indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		indexBufferView.SizeInBytes = sizeof(UINT) * indices;
	}
}

void Mesh::Render(ID3D12GraphicsCommandList* commandList)
{
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);

	if (indexBuffer)
	{
		commandList->IASetIndexBuffer(&indexBufferView);
		commandList->DrawIndexedInstanced(indices, 1, 0, 0, 0);
	}
	else
	{
		commandList->DrawInstanced(vertices, 1, 0, 0);
	}
}
