#pragma once

//class ostream;

class Vec3 {
public:
	Vec3() = default;
	Vec3(float x, float y, float z);
	~Vec3() = default;
public:
	float x{ 0.f };
	float y{ 0.f };
	float z{ 0.f };
public:
	const Vec3& operator+(const Vec3& rhs);
	const Vec3& operator+=(const Vec3& rhs);
	const Vec3& operator-(const Vec3& rhs);
	const Vec3& operator-=(const Vec3& rhs);
	const Vec3& operator*(const float& rhs);
	const Vec3& operator*=(const float& rhs);
	const Vec3& operator/(const float& rhs);
	const Vec3& operator/=(const float& rhs);
	float Length(const Vec3& vec);
	const Vec3& Normal(Vec3& vec);
};