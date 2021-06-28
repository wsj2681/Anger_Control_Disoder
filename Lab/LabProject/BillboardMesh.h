#pragma once
#include "Mesh.h"
class BillboardMesh : public Mesh
{
public:
	BillboardMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth, float fHeight, float fDepth, float fxPosition, float fyPosition, float fzPosition);
	virtual ~BillboardMesh();

public:

	XMFLOAT2* m_pxmf2TextureCoords0 = nullptr;
	ID3D12Resource* m_pd3dTextureCoord0Buffer = nullptr;
	ID3D12Resource* m_pd3dTextureCoord0UploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW			m_d3dTextureCoord0BufferView;

	void OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, void* pContext);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, int nSubSet);


};

