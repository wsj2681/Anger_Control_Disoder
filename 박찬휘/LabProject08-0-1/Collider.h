#pragma once

class Collider {
public:
	//Collider() = default;
	Collider(const float& sx = 1.0f, const float& sy = 1.0f, const float& sz = 1.0f, const float& px = 0.0f, const float& py = 0.0f, const float& pz = 0.0f);
	virtual ~Collider() = default;
protected:
	XMFLOAT3 size{ XMFLOAT3(1.0f, 1.0f, 1.0f) };
	XMFLOAT3 pos { XMFLOAT3(0.0f, 0.0f, 0.0f) };
	XMFLOAT3 max { XMFLOAT3(0.5f, 0.5f, 0.5f) };
	XMFLOAT3 min{ XMFLOAT3(-0.5f, -0.5f, -0.5f) };
	bool bCollision{ false };
public:
	virtual const XMFLOAT3& GetSize() const;
	virtual const XMFLOAT3& GetPosition() const;

	virtual void SetSize(const XMFLOAT3& size);
	virtual void SetPosition(const XMFLOAT3& pos);
};

class BoxCollider : public Collider {
public:
	BoxCollider(const float& sx = 1.0f, const float& sy = 1.0f, const float& sz = 1.0f, const float& px = 0.0f, const float& py = 0.0f, const float& pz = 0.0f);
	virtual ~BoxCollider() = default;
protected:
	
};