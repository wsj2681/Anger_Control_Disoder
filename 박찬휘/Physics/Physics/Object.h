#pragma once

class Object {
public:
	Object() = delete;
	// Object(const unsigned int& num, const XMFLOAT3& size, const XMFLOAT3& pos);
	Object(const unsigned int& num, const Point& size, const Point& pos);
	Object(const Object&) = delete;
	const Object& operator=(const Object&) = delete;
	~Object() = default;
protected:
	int num{ -1 };
	//XMFLOAT3 size{ 0.f, 0.f, 0.f };
	//XMFLOAT3 pos{ 0.f, 0.f, 0.f };
	Point size{ 0, 0 };
	Point pos{ 0, 0 };
	Physics physics;
public:
	/*void SetSize(const XMFLOAT3& size);
	const XMFLOAT3& GetSize() const;
	void SetPos(const XMFLOAT3& pos);
	const XMFLOAT3& GetPos() const;*/
	void SetSize(const Point& size);
	const Point& GetSize() const;
	void SetPos(const Point& pos);
	const Point& GetPos() const;

	virtual void PrintData(const char* name);
	virtual void Render();
	
};

class FixedObject : public Object {
public:
	//FixedObject(const unsigned int& num, const XMFLOAT3& size, const XMFLOAT3& pos);
	FixedObject(const unsigned int& num, const Point& size, const Point& pos);
public:
	virtual void Render();
};

class MovableObject : public Object {
public:
	// MovableObject(const unsigned int& num, const XMFLOAT3& size, const XMFLOAT3& pos);
	MovableObject(const unsigned int& num, const Point& size, const Point& pos);
public:
	// virtual void Move(const XMFLOAT3& dir);
	virtual void Move(const Point& dir);
	virtual void Render();
};

class PlayerObject : public MovableObject {
public:
	// PlayerObject(const unsigned int& num, const XMFLOAT3& size, const XMFLOAT3& pos);
	PlayerObject(const unsigned int& num, const Point& size, const Point& pos);
private:
	unsigned int hp{ 100 };
public:
	// virtual void Move(const XMFLOAT3& dir);
	virtual void Move(const Point& dir);
	virtual void Render();
};

class ParticleObject : public MovableObject {
public:
	ParticleObject(const unsigned int& num, const XMFLOAT3& size, const XMFLOAT3& pos);
	// ParticleObject(const unsigned int& num, const Point& size, const Point& pos);
public:
	virtual void Move(const XMFLOAT3& dir);
	// virtual void Move(const Point& dir);
	virtual void Render();
};