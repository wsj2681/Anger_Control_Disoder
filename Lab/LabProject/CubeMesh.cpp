#include "stdafx.h"
#include "CubeMesh.h"
#include "Object.h"

CubeMesh::CubeMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth, float fHeight, float fDepth)
	:Mesh(pd3dDevice, pd3dCommandList)
{
	Vector3::ScalarProduct(obb.Extents, 1.5f);
	obb.Extents.x += 0.5f;
	//������ü�� 6���� �� ����(x-��) ����
	m_nVertices = 36;
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//fWidth: ������ü ����(x-��) ����, fHeight: ������ü ����(y-��) ����, fDepth: ������ü ����(z-��) ����
	float fx = obb.Extents.x * 0.5f * 0.2f, fy = obb.Extents.y * 0.5f * 0.2f, fz = obb.Extents.z * 0.5f * 0.2f;

	CDiffusedVertex pVertices[36];

	int i = 0;

	//���� ���� �����ʹ� �ﰢ�� ����Ʈ�̹Ƿ� 36���� ���� �����͸� �غ��Ѵ�.

	//�� �ո�(Front) �簢���� ���� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//�� �ո�(Front) �簢���� �Ʒ��� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//�� ����(Top) �簢���� ���� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//�� ����(Top) �簢���� �Ʒ��� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//�� �޸�(Back) �簢���� ���� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//�� �޸�(Back) �簢���� �Ʒ��� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//�� �Ʒ���(Bottom) �簢���� ���� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//�� �Ʒ���(Bottom) �簢���� �Ʒ��� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//�� ����(Left) �簢���� ���� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//�� ����(Left) �簢���� �Ʒ��� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//�� ����(Right) �簢���� ���� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//�� ����(Right) �簢���� �Ʒ��� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });

	m_pd3dPositionBuffer =
		::CreateBufferResource(
			pd3dDevice,
			pd3dCommandList,
			pVertices,
			sizeof(CDiffusedVertex) * m_nVertices,
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
			&m_pd3dPositionUploadBuffer);

	//���� ���� �並 �����Ѵ�.
	m_d3dPositionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_d3dPositionBufferView.StrideInBytes = sizeof(CDiffusedVertex);
	m_d3dPositionBufferView.SizeInBytes = sizeof(CDiffusedVertex) * m_nVertices;


}

CubeMesh::CubeMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* name, float fWidth, float fHeight, float fDepth)
	:Mesh(pd3dDevice, pd3dCommandList)
{
	if (strcmp(name, "SpineLeft"))
	{

	}
	else if (strcmp(name, "SpineRight"))
	{

	}
	Vector3::ScalarProduct(obb.Extents, 1.5f);

	//������ü�� 6���� �� ����(x-��) ����
	m_nVertices = 36;
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//fWidth: ������ü ����(x-��) ����, fHeight: ������ü ����(y-��) ����, fDepth: ������ü ����(z-��) ����
	float fx = obb.Extents.x * 0.5f * 0.2f, fy = obb.Extents.y * 0.5f * 0.2f, fz = obb.Extents.z * 0.5f * 0.2f;

	CDiffusedVertex pVertices[36];

	int i = 0;

	//���� ���� �����ʹ� �ﰢ�� ����Ʈ�̹Ƿ� 36���� ���� �����͸� �غ��Ѵ�.

	//�� �ո�(Front) �簢���� ���� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//�� �ո�(Front) �簢���� �Ʒ��� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//�� ����(Top) �簢���� ���� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//�� ����(Top) �簢���� �Ʒ��� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//�� �޸�(Back) �簢���� ���� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//�� �޸�(Back) �簢���� �Ʒ��� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//�� �Ʒ���(Bottom) �簢���� ���� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//�� �Ʒ���(Bottom) �簢���� �Ʒ��� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//�� ����(Left) �簢���� ���� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//�� ����(Left) �簢���� �Ʒ��� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//�� ����(Right) �簢���� ���� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//�� ����(Right) �簢���� �Ʒ��� �ﰢ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });

	m_pd3dPositionBuffer =
		::CreateBufferResource(
			pd3dDevice,
			pd3dCommandList,
			pVertices,
			sizeof(CDiffusedVertex) * m_nVertices,
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
			&m_pd3dPositionUploadBuffer);

	//���� ���� �並 �����Ѵ�.
	m_d3dPositionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_d3dPositionBufferView.StrideInBytes = sizeof(CDiffusedVertex);
	m_d3dPositionBufferView.SizeInBytes = sizeof(CDiffusedVertex) * m_nVertices;


}

CubeMesh::~CubeMesh()
{
}

void CubeMesh::Update(Object* bone)
{
	this->obb.Center = bone->GetPosition();
}