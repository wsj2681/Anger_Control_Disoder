#include "stdafx.h"
#include "Collider.h"

Collider::Collider(const float& sx, const float& sy, const float& sz, const float& px, const float& py, const float& pz)
{
	size.x = sx;
	size.y = sy;
	size.z = sz;
	pos.x = px;
	pos.y = py;
	pos.z = pz;
}

const XMFLOAT3& Collider::GetSize() const
{
	return size;
}

const XMFLOAT3& Collider::GetPosition() const
{
	return pos;
}

void Collider::SetSize(const XMFLOAT3& size)
{
	this->size = size;
}

void Collider::SetPosition(const XMFLOAT3& pos)
{
	this->pos = pos;
}

BoxCollider::BoxCollider(const float& sx, const float& sy, const float& sz, const float& px, const float& py, const float& pz)
{
	size.x = sx;
	size.y = sy;
	size.z = sz;
	pos.x = px;
	pos.y = py;
	pos.z = pz;
}
