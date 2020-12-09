#pragma once
class Mesh
{
public:
	Mesh() = default;
	Mesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;
	virtual ~Mesh();

private:

	int references = 0;

protected:

	ID3D12Resource* vertexBuffer = nullptr;
	ID3D12Resource* vertexUploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	UINT slot = 0;
	UINT vertices = 0;
	UINT offset = 0;

	ID3D12Resource* indexBuffer = nullptr;
	ID3D12Resource* indexUploadBuffer = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	D3D12_PRIMITIVE_TOPOLOGY primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT indices = 0;
	UINT startIndex = 0;

public:

	void AddRefference();
	void Release();

	virtual void Render(ID3D12GraphicsCommandList* commandList);

	void ReleaseUploadBuffers();
};

