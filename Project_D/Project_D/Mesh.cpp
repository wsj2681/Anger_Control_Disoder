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

	//�޽��� ������Ƽ�� ������ �����Ѵ�.
	commandList->IASetPrimitiveTopology(this->primitiveTopology);

	//�޽��� ���� ���� �並 �����Ѵ�.
	commandList->IASetVertexBuffers(slot, 1, &vertexBufferView);

	if (indexBuffer)
	{
		// �ε��� ���۸� �Է������⿡ ����
		commandList->IASetIndexBuffer(&indexBufferView);
		commandList->DrawIndexedInstanced(indices, 1, 0, 0, 0);
	}
	else
	{
		commandList->DrawInstanced(vertices, 1, offset, 0);
	}
}

void Mesh::ReleaseUploadBuffers() {
	//���� ���۸� ���� ���ε� ���۸� �Ҹ��Ų��.
	if (vertexUploadBuffer) vertexUploadBuffer->Release();
	vertexUploadBuffer = nullptr;
};