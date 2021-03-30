#pragma once
#include "Mesh.h"

class StandardMesh : public Mesh
{
public:

	StandardMesh();
	StandardMesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	virtual ~StandardMesh();

protected:

	XMFLOAT4* colors = nullptr;
	XMFLOAT3* normals = nullptr;
	XMFLOAT3* tangents = nullptr;
	XMFLOAT3* biTangents = nullptr;

	XMFLOAT2* textureCoords0 = nullptr;
	XMFLOAT2* textureCoords1 = nullptr;

	ID3D12Resource* textureCoord0Buffer = nullptr;
	ID3D12Resource* textureCoord0UploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW textureCoord0BufferView;

	ID3D12Resource* textureCoord1Buffer = nullptr;
	ID3D12Resource* textureCoord1UploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW textureCoord1BufferView;

	ID3D12Resource* normalBuffer = nullptr;
	ID3D12Resource* normalUploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW normalBufferView;

	ID3D12Resource* tangentBuffer = nullptr;
	ID3D12Resource* tangentUploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW tangentBufferView;

	ID3D12Resource* biTangentBuffer = nullptr;
	ID3D12Resource* biTangentUploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW biTangentBufferView;

public:

	void LoadMeshFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile);
	virtual void ReleaseUploadBuffers();
	virtual void OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, void* pContext);

};

