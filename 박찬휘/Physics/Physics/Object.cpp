#include "CommonHeader.h"
#include "Physics.h"
#include "Object.h"

//Object::Object(const unsigned int& num, const XMFLOAT3& size, const XMFLOAT3& pos)
//{
//	this->num = num;
//	this->size = size;
//	this->pos = pos;
//	PrintXMFLOAT3("size", size);
//}

Object::Object(const unsigned int& num, const Point& size, const Point& pos)
{
	this->num = num;
	this->size = size;
	this->pos = pos;
	// PrintPoint("size", size);
}

//void Object::SetSize(const XMFLOAT3& size)
//{
//	this->size = size;
//}
//
//const XMFLOAT3& Object::GetSize() const
//{
//	return size;n
//}
//
//void Object::SetPos(const XMFLOAT3& pos)
//{
//	this->pos = pos;
//}
//
//const XMFLOAT3& Object::GetPos() const
//{
//	return pos;
//}

void Object::SetSize(const Point& size)
{
	this->size = size;
}

const Point& Object::GetSize() const
{
	return size;
}

void Object::SetPos(const Point& pos)
{
	this->pos = pos;
}

const Point& Object::GetPos() const
{
	return pos;
}

void Object::PrintData(const char* name)
{
	cout << "[" << name << "] num : " << num << ", ";
	// PrintXMFLOAT3("pos ", pos);
	PrintPoint("pos ", pos);
}

void Object::Render()
{
	PrintData("Object");
}

//FixedObject::FixedObject(const unsigned int& num, const XMFLOAT3& size, const XMFLOAT3& pos) : Object(num, size, pos)
//{
//	
//}

FixedObject::FixedObject(const unsigned int& num, const Point& size, const Point& pos) : Object(num, size, pos)
{

}

void FixedObject::Render()
{
	PrintData("FixedObject");
}

//MovableObject::MovableObject(const unsigned int& num, const XMFLOAT3& size, const XMFLOAT3& pos) : Object(num, size, pos)
//{
//
//}

MovableObject::MovableObject(const unsigned int& num, const Point& size, const Point& pos) : Object(num, size, pos)
{

}


//void MovableObject::Move(const XMFLOAT3& dir)
//{
//	SetPos(this->pos + dir);
//}

void MovableObject::Move(const Point& dir)
{
	SetPos(this->pos + dir);
}

void MovableObject::Render()
{
	PrintData("MovableObject");
}

//PlayerObject::PlayerObject(const unsigned int& num, const XMFLOAT3& size, const XMFLOAT3& pos) : MovableObject(num, size, pos)
//{
//
//}

PlayerObject::PlayerObject(const unsigned int& num, const Point& size, const Point& pos) : MovableObject(num, size, pos)
{

}

//void PlayerObject::Move(const XMFLOAT3& dir)
//{
//	SetPos(this->pos + dir);
//}

void PlayerObject::Move(const Point& dir)
{
	SetPos(this->pos + dir);
}

void PlayerObject::Render()
{
	PrintData("PlayerObject");
	
}

//ParticleObject::ParticleObject(const unsigned int& num, const XMFLOAT3& size, const XMFLOAT3& pos) : MovableObject(num, size, pos)
//{
//
//}

ParticleObject::ParticleObject(const unsigned int& num, const Point& size, const Point& pos) : MovableObject(num, size, pos)
{

}

//void ParticleObject::Move(const XMFLOAT3& dir)
//{
//	SetPos(this->pos + dir);
//}

void ParticleObject::Move(const Point& dir)
{
	SetPos(this->pos + dir);
}

void ParticleObject::Render()
{
	PrintData("ParticleObject");
}