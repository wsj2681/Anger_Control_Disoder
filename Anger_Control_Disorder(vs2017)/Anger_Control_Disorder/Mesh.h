#pragma once

// 기본 꼭지점
class Vertex
{
public:
	// Special Member Functions
	Vertex();
	Vertex(XMFLOAT3 xmf3Pos);
	Vertex(const Vertex&) = delete;
	Vertex& operator=(const Vertex&) = delete;
	~Vertex() = default;
protected:
	// Class Member Variables
	XMFLOAT3 xmf3Pos;
public:
	// Get & Set
	const XMFLOAT3& Getxmf3Pos() const;
	void Setxmf3Pos(const float& x, const float& y, const float& z);
	void Setxmf3Pos(const XMFLOAT3& xmf3Pos);
};

// 퍼진 꼭지점
class DiffusedVertex : public Vertex
{
public:
	// Special Member Functions
	DiffusedVertex();
	DiffusedVertex(float x, float y, float z, XMFLOAT4 xmf4Dif);
	DiffusedVertex(XMFLOAT3 xmf3Pos, XMFLOAT4 xmf4Dif);
	DiffusedVertex(const DiffusedVertex&) = delete;
	DiffusedVertex& operator=(const DiffusedVertex&) = delete;
	~DiffusedVertex() = default;
protected:
	// Class Member Variables
	XMFLOAT4 xmf4Dif;
public:
	// Get & Set
	const XMFLOAT4& Getxmf4Dif() const;
	void Setxmf4Dif(const XMFLOAT4& xmf4Dif);
};

// 텍스쳐링 된? 텍스쳐 좌표 1개인 꼭지점
class TexturedVertex : public Vertex
{
public:
	// Special Member Functions
	TexturedVertex();
	TexturedVertex(float x, float y, float z, XMFLOAT2 xmf2TexCoord);
	TexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT2 xmf2TexCoord);
	TexturedVertex(const TexturedVertex&) = delete;
	TexturedVertex& operator=(const TexturedVertex&) = delete;
	~TexturedVertex() = default;
protected:
	// Class Member Variables
	XMFLOAT2 xmf2TexCoord;
public:
	// Get & Set
	const XMFLOAT2& Getxmf2TexCoord() const;
	void Setxmf2TexCoord(const XMFLOAT2& xmf2TexCoord);
};

// 퍼진 텍스쳐링 된? 텍스쳐 좌표 1개인 꼭지점
class DiffusedTexturedVertex : public DiffusedVertex
{
public:
	// Special Member Functions
	DiffusedTexturedVertex();
	DiffusedTexturedVertex(float x, float y, float z, XMFLOAT4 xmf4Dif, XMFLOAT2 xmf2TexCoord);
	DiffusedTexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse, XMFLOAT2 xmf2TexCoord);
	~DiffusedTexturedVertex() { }
protected:
	// Class Member Variables
	XMFLOAT2 xmf2TexCoord;
public:
	// Get & Set
	const XMFLOAT2& Getxmf2TexCoord() const;
	void Setxmf2TexCoord(const XMFLOAT2& xmf2TexCoord);
};

// 퍼진 텍스쳐링 된? 텍스쳐 좌표 2개인 꼭지점 
class Diffused2TexturedVertex : public DiffusedVertex
{
public:
	// Special Member Functions
	Diffused2TexturedVertex();
	Diffused2TexturedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse, XMFLOAT2 xmf2TexCoord0, XMFLOAT2 xmf2TexCoord1);
	Diffused2TexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse, XMFLOAT2 xmf2TexCoord0, XMFLOAT2 xmf2TexCoord1);
	Diffused2TexturedVertex(const Diffused2TexturedVertex&) = delete;
	Diffused2TexturedVertex& operator=(const Diffused2TexturedVertex&) = delete;
	~Diffused2TexturedVertex() = default;
protected:
	// Class Member Variables
	XMFLOAT2 xmf2TexCoord0;
	XMFLOAT2 xmf2TexCoord1;
public:
	// Get & Set
	const XMFLOAT2& Getxmf2TexCoord0() const;
	const XMFLOAT2& Getxmf2TexCoord1() const;
	void Setxmf2TexCoord0(const XMFLOAT2& xmf2TexCoord0);
	void Setxmf2TexCoord1(const XMFLOAT2& xmf2TexCoord1);
};

// 기하 빌보드 꼭지점
class CGeometryBillboardVertex : public Vertex
{
public:
	// Special Member Functions
	CGeometryBillboardVertex();
	CGeometryBillboardVertex(float x, float y, float z, XMFLOAT2 xmf2Size, UINT nTexture);
	CGeometryBillboardVertex(XMFLOAT3 xmf3Position, XMFLOAT2 xmf2Size, UINT nTexture);
	CGeometryBillboardVertex(const CGeometryBillboardVertex&) = delete;
	CGeometryBillboardVertex& operator=(const CGeometryBillboardVertex&) = delete;
	~CGeometryBillboardVertex() = default;
protected:
	// Class Member Variables
	XMFLOAT2 xmf2Size;
	UINT	 nTexture;
public:
	// Get & Set
	const XMFLOAT2& Getxmf2Size() const;
	const UINT& GetnTexture() const;
	void Setxmf2Size(const XMFLOAT2& xmf2TexCoord0);
	void SetnTexture(const UINT& xmf2TexCoord1);
};