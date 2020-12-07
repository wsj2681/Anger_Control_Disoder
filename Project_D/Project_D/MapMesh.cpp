#include "ResourceHelper.h"
#include "Mesh.h"
#include "MapMesh.h"
#include "DiffusedVertex.h"


MapMesh::MapMesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, void* meshData)
{
	vertexBuffer;
	indexBuffer;


	vertexBuffer = ::CreateBufferResource
	(
		device, commandList, nullptr/*DATA*/, 0/*sizeof DATA*/,
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		&vertexUploadBuffer
	);

	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(DiffusedVertex);
	vertexBufferView.SizeInBytes = sizeof(DiffusedVertex) * vertices;


	indexBuffer = ::CreateBufferResource
	(
		device, commandList, nullptr/*DATA*/, 0/*sizeof DATA*/,
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		&indexUploadBuffer
	);

	indexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexBufferView.SizeInBytes = sizeof(UINT) * indices;
}
