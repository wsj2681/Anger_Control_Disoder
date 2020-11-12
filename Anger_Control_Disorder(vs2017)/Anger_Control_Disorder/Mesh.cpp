#include "framework.h"
#include "Mesh.h"

//------------------------------
Vertex::Vertex()
//------------------------------
{
	this->xmf3Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

//------------------------------
Vertex::Vertex(XMFLOAT3 xmf3Pos)
//------------------------------
{
	this->xmf3Pos = xmf3Pos;
}

//------------------------------
const XMFLOAT3& Vertex::Getxmf3Pos() const
//------------------------------
{
	return this->xmf3Pos;
}

//------------------------------
void Vertex::Setxmf3Pos(const float& x, const float& y, const float& z)
//------------------------------
{
	this->xmf3Pos = XMFLOAT3(x, y, z);
}

//------------------------------
void Vertex::Setxmf3Pos(const XMFLOAT3& xmf3Pos)
//------------------------------
{
	this->xmf3Pos = xmf3Pos;
}

//------------------------------
DiffusedVertex::DiffusedVertex()
//------------------------------
{
	this->xmf3Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->xmf4Dif = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
}

//------------------------------
DiffusedVertex::DiffusedVertex(float x, float y, float z, XMFLOAT4 xmf4Dif)
//------------------------------
{
	this->xmf3Pos = XMFLOAT3(x, y, z);
	this->xmf4Dif = xmf4Dif;
}

//------------------------------
DiffusedVertex::DiffusedVertex(XMFLOAT3 xmf3Pos, XMFLOAT4 xmf4Dif)
//------------------------------
{
	this->xmf3Pos = xmf3Pos;
	this->xmf4Dif = xmf4Dif;
}

//------------------------------
const XMFLOAT4& DiffusedVertex::Getxmf4Dif() const
//------------------------------
{
	return this->xmf4Dif;
}

//------------------------------
void DiffusedVertex::Setxmf4Dif(const XMFLOAT4& xmf4Dif)
//------------------------------
{
	this->xmf4Dif = xmf4Dif;
}

//------------------------------
TexturedVertex::TexturedVertex()
//------------------------------
{
	this->xmf3Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->xmf2TexCoord = XMFLOAT2(0.0f, 0.0f);
}

//------------------------------
TexturedVertex::TexturedVertex(float x, float y, float z, XMFLOAT2 xmf2TexCoord)
//------------------------------
{
	this->xmf3Pos = XMFLOAT3(x, y, z);
	this->xmf2TexCoord = xmf2TexCoord;
}

//------------------------------
TexturedVertex::TexturedVertex(XMFLOAT3 xmf3Pos, XMFLOAT2 xmf2TexCoord)
//------------------------------
{
	this->xmf3Pos = xmf3Pos;
	this->xmf2TexCoord = xmf2TexCoord;
}

//------------------------------
const XMFLOAT2& TexturedVertex::Getxmf2TexCoord() const
//------------------------------
{
	return this->xmf2TexCoord;
}

//------------------------------
void TexturedVertex::Setxmf2TexCoord(const XMFLOAT2& xmf2TexCoord)
//------------------------------
{
	this->xmf2TexCoord = xmf2TexCoord;
}

//------------------------------
DiffusedTexturedVertex::DiffusedTexturedVertex()
//------------------------------
{
	this->xmf3Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->xmf4Dif = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	this->xmf2TexCoord = XMFLOAT2(0.0f, 0.0f);
}

//------------------------------
DiffusedTexturedVertex::DiffusedTexturedVertex(float x, float y, float z, XMFLOAT4 xmf4Dif, XMFLOAT2 xmf2TexCoord)
//------------------------------
{
	this->xmf3Pos = XMFLOAT3(x, y, z);
	this->xmf4Dif = xmf4Dif;
	this->xmf2TexCoord = xmf2TexCoord;
}

//------------------------------
DiffusedTexturedVertex::DiffusedTexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Dif, XMFLOAT2 xmf2TexCoord)
//------------------------------
{
	this->xmf3Pos = xmf3Position;
	this->xmf4Dif = xmf4Dif;
	this->xmf2TexCoord = xmf2TexCoord;
}

//------------------------------
const XMFLOAT2& DiffusedTexturedVertex::Getxmf2TexCoord() const
//------------------------------
{
	return this->xmf2TexCoord;
}

//------------------------------
void DiffusedTexturedVertex::Setxmf2TexCoord(const XMFLOAT2& xmf2TexCoord)
//------------------------------
{
	this->xmf2TexCoord = xmf2TexCoord;
}

//------------------------------
Diffused2TexturedVertex::Diffused2TexturedVertex()
//------------------------------
{
	this->xmf3Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->xmf4Dif = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	this->xmf2TexCoord0 = this->xmf2TexCoord1 = XMFLOAT2(0.0f, 0.0f);
}

//------------------------------
Diffused2TexturedVertex::Diffused2TexturedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse, XMFLOAT2 xmf2TexCoord0, XMFLOAT2 xmf2TexCoord1)
//------------------------------
{
	this->xmf3Pos = XMFLOAT3(x, y, z);
	this->xmf4Dif = xmf4Diffuse;
	this->xmf2TexCoord0 = xmf2TexCoord0;
	this->xmf2TexCoord1 = xmf2TexCoord1;
}

//------------------------------
Diffused2TexturedVertex::Diffused2TexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse, XMFLOAT2 xmf2TexCoord0, XMFLOAT2 xmf2TexCoord1)
//------------------------------
{
	this->xmf3Pos = xmf3Position;
	this->xmf4Dif = xmf4Diffuse;
	this->xmf2TexCoord0 = xmf2TexCoord0;
	this->xmf2TexCoord1 = xmf2TexCoord1;
}

//------------------------------
const XMFLOAT2& Diffused2TexturedVertex::Getxmf2TexCoord0() const
//------------------------------
{
	return this->xmf2TexCoord0;
}

//------------------------------
const XMFLOAT2& Diffused2TexturedVertex::Getxmf2TexCoord1() const
//------------------------------
{
	return this->xmf2TexCoord1;
}

//------------------------------
void Diffused2TexturedVertex::Setxmf2TexCoord0(const XMFLOAT2& xmf2TexCoord0)
//------------------------------
{
	this->xmf2TexCoord0 = xmf2TexCoord0;
}

//------------------------------
void Diffused2TexturedVertex::Setxmf2TexCoord1(const XMFLOAT2& xmf2TexCoord1)
//------------------------------
{
	this->xmf2TexCoord1 = xmf2TexCoord1;
}

//------------------------------
CGeometryBillboardVertex::CGeometryBillboardVertex()
//------------------------------
{
	this->xmf3Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->xmf2Size = XMFLOAT2(5.0f, 10.0f);
	this->nTexture = 0;
}

//------------------------------
CGeometryBillboardVertex::CGeometryBillboardVertex(float x, float y, float z, XMFLOAT2 xmf2Size, UINT nTexture)
//------------------------------
{
	this->xmf3Pos = XMFLOAT3(x, y, z);
	this->xmf2Size = xmf2Size;
	this->nTexture = nTexture;
}

//------------------------------
CGeometryBillboardVertex::CGeometryBillboardVertex(XMFLOAT3 xmf3Position, XMFLOAT2 xmf2Size, UINT nTexture)
//------------------------------
{
	this->xmf3Pos = xmf3Position;
	this->xmf2Size = xmf2Size;
	this->nTexture = nTexture;
}

//------------------------------
const XMFLOAT2& CGeometryBillboardVertex::Getxmf2Size() const
//------------------------------
{
	return this->xmf2Size;
}

//------------------------------
const UINT& CGeometryBillboardVertex::GetnTexture() const
//------------------------------
{
	return this->nTexture;
}

//------------------------------
void CGeometryBillboardVertex::Setxmf2Size(const XMFLOAT2& xmf2Size)
//------------------------------
{
	this->xmf2Size = xmf2Size;
}

//------------------------------
void CGeometryBillboardVertex::SetnTexture(const UINT& nTexture)
//------------------------------
{
	this->nTexture = nTexture;
}