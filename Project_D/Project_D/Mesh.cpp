#include "framework.h"
#include "Vertex.h"
#include "Mesh.h"

Mesh::Mesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
}

Mesh::~Mesh()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(vertexUploadBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(indexUploadBuffer);
}

void Mesh::AddRefference()
{
	this->references++;
}

void Mesh::Release()
{
	if (--this->references <= 0)
		delete this;
}

void Mesh::Render(ID3D12GraphicsCommandList* commandList)
{
	ASSERT(commandList != nullptr, "Mesh::Render pd3dCommandList is nullptr.");

	//메쉬의 프리미티브 유형을 설정한다.
	commandList->IASetPrimitiveTopology(this->primitiveTopology);

	//메쉬의 정점 버퍼 뷰를 설정한다.
	commandList->IASetVertexBuffers(slot, 1, &vertexBufferView);

	if (indexBuffer)
	{
		// 인덱스 버퍼를 입력조립기에 연결
		commandList->IASetIndexBuffer(&indexBufferView);
		commandList->DrawIndexedInstanced(indices, 1, 0, 0, 0);
	}
	else
	{
		commandList->DrawInstanced(vertices, 1, offset, 0);
	}
}

void Mesh::ReleaseUploadBuffers() {
	//정점 버퍼를 위한 업로드 버퍼를 소멸시킨다.
	if (vertexUploadBuffer) vertexUploadBuffer->Release();
	vertexUploadBuffer = nullptr;
};