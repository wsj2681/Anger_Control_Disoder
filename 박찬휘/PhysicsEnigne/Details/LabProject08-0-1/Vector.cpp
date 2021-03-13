#include <cmath>
#include "stdafx.h"
#include "Vector.h"

Vec3::Vec3(float x, float y, float z) : x{ x }, y{ y }, z{ z } {

}

const Vec3& Vec3::operator+(const Vec3& rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;
	return *this;
}

const Vec3& Vec3::operator+=(const Vec3& rhs)
{
	return this->operator+(rhs);
}

const Vec3& Vec3::operator-(const Vec3& rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;
	return *this;
}

const Vec3& Vec3::operator-=(const Vec3& rhs)
{
	return this->operator-(rhs);
}

const Vec3& Vec3::operator*(const float& rhs)
{
	this->x *= rhs;
	this->y *= rhs;
	this->z *= rhs;
	return *this;
}

const Vec3& Vec3::operator*=(const float& rhs)
{
	return this->operator*(rhs);
}

const Vec3& Vec3::operator/(const float& rhs)
{
	this->x /= rhs;
	this->y /= rhs;
	this->z /= rhs;
	return *this;
}

const Vec3& Vec3::operator/=(const float& rhs)
{
	return this->operator/(rhs);
}

float Vec3::Length(const Vec3& vec)
{
	return sqrtf(powf(vec.x, 2) + powf(vec.y, 2) + powf(vec.z, 2));
}

const Vec3& Vec3::Normal(Vec3& vec)
{
	float len = this->Length(vec);

	return vec / len;
}
