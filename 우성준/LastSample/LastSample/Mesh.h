#pragma once

struct Vertex
{
	Vertex() = default;
	Vertex(float x, float y, float z) { position.x = x; position.y = y; position.z = z; }
	Vertex(const XMFLOAT3& position) { this->position = position; }
	virtual ~Vertex() = default;
	
	XMFLOAT3 position;

};

struct DiffusedVertex : public Vertex
{
	DiffusedVertex() = default;
	DiffusedVertex(float x, float y, float z, float r, float g, float b, float a) 
	{ 
		position.x = x; position.y = y; position.z = z; 
		diffuse.x = r; diffuse.y = g; diffuse.z = b; diffuse.w = a;
	}
	DiffusedVertex(float x, float y, float z, const XMFLOAT4& diffuse) 
	{ 
		position.x = x; position.y = y; position.z = z; 
		this->diffuse = diffuse; 
	}
	DiffusedVertex(const XMFLOAT3& position, const XMFLOAT4& diffuse) 
	{ 
		this->position = position; this->diffuse = diffuse;
	}
	virtual ~DiffusedVertex() = default;

	XMFLOAT4 diffuse;
};

struct TexturedVertes : public Vertex
{
	TexturedVertes() = default;
	TexturedVertes(float x, float y, float z, float u, float v) 
	{ 
		position.x = x; position.y = y; position.z = z; 
		texCoord.x = u; texCoord.y = v;
	}
	TexturedVertes(float x, float y, float z, const XMFLOAT2& texCoord) 
	{ 
		position.x = x; position.y = y; position.z = z; 
		this->texCoord = texCoord; 
	}
	TexturedVertes(const XMFLOAT3& position, const XMFLOAT2& texCoord)
	{
		this->position = position;
		this->texCoord = texCoord;
	}
	virtual ~TexturedVertes() = default;

	XMFLOAT2 texCoord;
};

struct DiffusedTexturedVertex : public DiffusedVertex
{
	DiffusedTexturedVertex() = default;
	DiffusedTexturedVertex(
		float x, float y, float z, 
		float r, float g, float b, float a, 
		float u, float v)
	{
		position.x = x; position.y = y; position.z = z;
		diffuse.x = r; diffuse.y = g; diffuse.z = b; diffuse.w = a;
		texCoord.x = u; texCoord.y = v;
	}
	DiffusedTexturedVertex(float x, float y, float z, const XMFLOAT4& diffuse,const XMFLOAT2 & texCoord)
	{
		position.x = x; position.y = y; position.z = z;
		this->diffuse = diffuse;
		this->texCoord = texCoord;
	}
	DiffusedTexturedVertex(const XMFLOAT3 & position, const XMFLOAT4& diffuse,const XMFLOAT2 & texCoord)
	{
		this->position = position;
		this->diffuse = diffuse;
		this->texCoord = texCoord;
	}
	virtual ~DiffusedTexturedVertex() = default;

	XMFLOAT2 texCoord;
};


class Mesh
{
public:
	Mesh() = default;
	Mesh(const Mesh&) = delete;
	Mesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	Mesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nVertices, XMFLOAT3* pxmf3Positions, UINT nIndices, UINT* pnIndices);
	Mesh& operator=(const Mesh&) = delete;
	virtual ~Mesh() = default;

protected:

	UINT vertices = 0;
	ID3D12Resource* vertexBuffer = nullptr;
	ID3D12Resource* vertexUploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	UINT indices = 0;
	ID3D12Resource* indexBuffer = nullptr;
	ID3D12Resource* indexUploadBuffer = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};


public:

	virtual void Render(ID3D12GraphicsCommandList* commandList);

};

