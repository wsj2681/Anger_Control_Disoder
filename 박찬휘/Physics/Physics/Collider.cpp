#include "CommonHeader.h"
#include "RigidBody.h"


enum ColliderType
{
	Box,
	Sphere
};

class Collider
{
protected:

	XMFLOAT3 transform;

public:

	ColliderType colliderType;
	RigidBody* rigidBody;
	XMFLOAT3  offset;

	const XMFLOAT3& GetTransform() const
	{
		return transform;
	}

	Vector3 GetAxis(unsigned index) const
	{
		return transform.GetAxisVector(index);
	}

	void calculateInternals()
	{
		this->transform = rigidBody->GetTransform() * offset;
	}

	virtual ~Collider()
	{

	}

};


class SphereCollider : public Collider
{
public:

	real radius;

	SphereCollider()
	{
		Collider::colliderType = ColliderType::Sphere;
	}
};


class BoxCollider : public Collider
{
public:

	Vector3 halfSize;

	BoxCollider()
	{
		Collider::colliderType = ColliderType::Box;
	}
};
