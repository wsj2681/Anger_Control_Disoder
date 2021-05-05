#pragma once
#include "Mesh.h"
class SkyBoxMesh : public Mesh
{
public:
	SkyBoxMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 20.0f);
	virtual ~SkyBoxMesh();
};