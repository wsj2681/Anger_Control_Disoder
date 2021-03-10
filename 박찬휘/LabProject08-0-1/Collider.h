//#pragma once
//
//enum ColliderType
//{
//	Box,
//	Sphere
//};
//
//class Collider
//{
//protected:
//
//	XMFLOAT4X4 transform;
//
//public:
//	ColliderType colliderType;
//	RigidBody* rigidBody;
//	XMFLOAT4X4  offset;
//
//	const XMFLOAT4X4& GetTransform() const;
//	XMFLOAT3 GetAxis(unsigned index) const;
//	void calculateInternals();
//	virtual ~Collider();
//
//};
//
//
//class SphereCollider : public Collider
//{
//public:
//	real radius;
//
//	SphereCollider();
//};
//
//
//class BoxCollider : public Collider
//{
//public:
//	XMFLOAT3 halfSize;
//
//	BoxCollider();
//};
