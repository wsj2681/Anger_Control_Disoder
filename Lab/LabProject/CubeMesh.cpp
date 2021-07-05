#include "stdafx.h"
#include "CubeMesh.h"
#include "Object.h"

CubeMesh::CubeMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth, float fHeight, float fDepth)
	:Mesh(pd3dDevice, pd3dCommandList)
{
	Vector3::ScalarProduct(obb.Extents, 1.5f);
	obb.Extents.x += 0.5f;
	//직육면체는 6개의 면 가로(x-축) 길이
	m_nVertices = 36;
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//fWidth: 직육면체 가로(x-축) 길이, fHeight: 직육면체 세로(y-축) 길이, fDepth: 직육면체 깊이(z-축) 길이
	float fx = obb.Extents.x * 0.5f * 0.2f, fy = obb.Extents.y * 0.5f * 0.2f, fz = obb.Extents.z * 0.5f * 0.2f;

	CDiffusedVertex pVertices[36];

	int i = 0;

	//정점 버퍼 데이터는 삼각형 리스트이므로 36개의 정점 데이터를 준비한다.

	//ⓐ 앞면(Front) 사각형의 위쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓑ 앞면(Front) 사각형의 아래쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓒ 윗면(Top) 사각형의 위쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓓ 윗면(Top) 사각형의 아래쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓔ 뒷면(Back) 사각형의 위쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓕ 뒷면(Back) 사각형의 아래쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓖ 아래면(Bottom) 사각형의 위쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓗ 아래면(Bottom) 사각형의 아래쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓘ 옆면(Left) 사각형의 위쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓙ 옆면(Left) 사각형의 아래쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓚ 옆면(Right) 사각형의 위쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓛ 옆면(Right) 사각형의 아래쪽 삼각형
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

	//정점 버퍼 뷰를 생성한다.
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

	//직육면체는 6개의 면 가로(x-축) 길이
	m_nVertices = 36;
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//fWidth: 직육면체 가로(x-축) 길이, fHeight: 직육면체 세로(y-축) 길이, fDepth: 직육면체 깊이(z-축) 길이
	float fx = obb.Extents.x * 0.5f * 0.2f, fy = obb.Extents.y * 0.5f * 0.2f, fz = obb.Extents.z * 0.5f * 0.2f;

	CDiffusedVertex pVertices[36];

	int i = 0;

	//정점 버퍼 데이터는 삼각형 리스트이므로 36개의 정점 데이터를 준비한다.

	//ⓐ 앞면(Front) 사각형의 위쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓑ 앞면(Front) 사각형의 아래쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓒ 윗면(Top) 사각형의 위쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓓ 윗면(Top) 사각형의 아래쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓔ 뒷면(Back) 사각형의 위쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓕ 뒷면(Back) 사각형의 아래쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓖ 아래면(Bottom) 사각형의 위쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓗ 아래면(Bottom) 사각형의 아래쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓘ 옆면(Left) 사각형의 위쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓙ 옆면(Left) 사각형의 아래쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓚ 옆면(Right) 사각형의 위쪽 삼각형
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), { 0.f, 0.f, 1.f, 1.f });
	//ⓛ 옆면(Right) 사각형의 아래쪽 삼각형
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

	//정점 버퍼 뷰를 생성한다.
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