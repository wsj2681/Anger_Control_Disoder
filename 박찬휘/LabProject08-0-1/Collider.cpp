////#include "GameFramework.h"
//#include "stdafx.h"
//#include "RigidBody.h"
//#include "Collider.h"
//
//	const XMFLOAT4X4& Collider::GetTransform() const
//	{
//		return transform;
//	}
//
//	XMFLOAT3 Collider::GetAxis(unsigned index) const
//	{
//		return transform.GetAxisVector(index);
//	}
//
//	void Collider::calculateInternals()
//	{
//		this->transform = rigidBody->GetTransform() * offset;
//	}
//
//	Collider::~Collider()
//	{
//
//	}
//
//
//	SphereCollider::SphereCollider()
//	{
//		Collider::colliderType = ColliderType::Sphere;
//	}
//
//	BoxCollider::BoxCollider()
//	{
//		Collider::colliderType = ColliderType::Box;
//	}