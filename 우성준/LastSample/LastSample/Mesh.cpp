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

CubeMesh::CubeMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth, float fHeight, float fDepth)
{
	vertices = 8;

	float fx = fWidth * 0.5f, fy = fHeight * 0.5f, fz = fDepth * 0.5f;

	Vertex pVertices[8];

	pVertices[0] = Vertex(XMFLOAT3(-fx, +fy, -fz));
	pVertices[1] = Vertex(XMFLOAT3(+fx, +fy, -fz));
	pVertices[2] = Vertex(XMFLOAT3(+fx, +fy, +fz));
	pVertices[3] = Vertex(XMFLOAT3(-fx, +fy, +fz));
	pVertices[4] = Vertex(XMFLOAT3(-fx, -fy, -fz));
	pVertices[5] = Vertex(XMFLOAT3(+fx, -fy, -fz));
	pVertices[6] = Vertex(XMFLOAT3(+fx, -fy, +fz));
	pVertices[7] = Vertex(XMFLOAT3(-fx, -fy, +fz));

	vertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices, sizeof(Vertex) * vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &vertexUploadBuffer);
	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(Vertex);
	vertexBufferView.SizeInBytes = sizeof(Vertex) * vertices;

	indices = 36;

	UINT pnIndices[36];

	pnIndices[0] = 3; pnIndices[1] = 1; pnIndices[2] = 0;
	pnIndices[3] = 2; pnIndices[4] = 1; pnIndices[5] = 3;
	pnIndices[6] = 0; pnIndices[7] = 5; pnIndices[8] = 4;
	pnIndices[9] = 1; pnIndices[10] = 5; pnIndices[11] = 0;
	pnIndices[12] = 3; pnIndices[13] = 4; pnIndices[14] = 7;
	pnIndices[15] = 0; pnIndices[16] = 4; pnIndices[17] = 3;
	pnIndices[18] = 1; pnIndices[19] = 6; pnIndices[20] = 5;
	pnIndices[21] = 2; pnIndices[22] = 6; pnIndices[23] = 1;
	pnIndices[24] = 2; pnIndices[25] = 7; pnIndices[26] = 6;
	pnIndices[27] = 3; pnIndices[28] = 7; pnIndices[29] = 2;
	pnIndices[30] = 6; pnIndices[31] = 4; pnIndices[32] = 5;
	pnIndices[33] = 7; pnIndices[34] = 4; pnIndices[35] = 6;

	indexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pnIndices, sizeof(UINT) * indices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &indexUploadBuffer);
	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexBufferView.SizeInBytes = sizeof(UINT) * indices;
}
