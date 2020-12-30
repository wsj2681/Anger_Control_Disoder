#include "framework.h"
#include "DiffusedVertex.h"

DiffusedVertex::DiffusedVertex(const XMFLOAT3& position, const XMFLOAT4& diffuse)
{
	this->position = position;
	this->diffuseColor = diffuse;
}

DiffusedVertex::DiffusedVertex(float x, float y, float z, const XMFLOAT4& diffuse)
{
	this->position = XMFLOAT3(x, y, z);
	this->diffuseColor = diffuse;
}

DiffusedVertex::DiffusedVertex(float x, float y, float z, float r, float g, float b, float a)
{
	this->position = XMFLOAT3(x, y, z);
	this->diffuseColor = XMFLOAT4(r, g, b, a);
}