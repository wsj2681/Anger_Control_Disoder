#pragma once
#include "Mesh.h"
class CubeMesh : public Mesh
{
public:
	CubeMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth = 10.0f, float fHeight = 10.0f, float fDepth = 10.0f);
	CubeMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* name, float fWidth = 10.0f, float fHeight = 10.0f, float fDepth = 10.0f);
	virtual ~CubeMesh();

private:

	ID3D12Resource* m_pd3dIndexBuffer = NULL;
	ID3D12Resource* m_pd3dIndexUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW m_d3dIndexBufferView;

public:

	virtual void Update(Object* bone);

};
