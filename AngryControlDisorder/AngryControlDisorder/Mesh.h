#pragma once
class Mesh
{
public:

	Mesh();
	Mesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;
	virtual ~Mesh();

private:

	int references = 0;

protected:

	char meshName[64];

	UINT type = 0x00;

	XMFLOAT3 AABBCenter{ 0.f, 0.f, 0.f };
	XMFLOAT3 AABBExtents{ 0.f, 0.f, 0.f };

	D3D12_PRIMITIVE_TOPOLOGY primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT slot = 0;
	UINT offset = 0;

	int vertices = 0;
	XMFLOAT3* positions = nullptr;

	ID3D12Resource* positionBuffer = nullptr;
	ID3D12Resource* positionUploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW positionBufferView;

	int	subMeshes = 0;
	int* subSetIndiceCount = nullptr;
	UINT** subSetIndices = nullptr;

	ID3D12Resource** subSetIndexBuffers = nullptr;
	ID3D12Resource** subSetIndexUploadBuffers = nullptr;
	D3D12_INDEX_BUFFER_VIEW* subSetIndexBufferViews = nullptr;

public:

	void AddRef();
	void Release();

	virtual void CreatShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* commandList);
	virtual void ReleaseShaderVariables();

	virtual void ReleaseUploadBuffers();

	virtual void OnPreRender(ID3D12GraphicsCommandList* commandList, void* pContext);
	virtual void Render(ID3D12GraphicsCommandList* commandList, int nSubSet);
	virtual void OnPostRender(ID3D12GraphicsCommandList* commandList, void* pContext);

	const UINT GetType();
};

