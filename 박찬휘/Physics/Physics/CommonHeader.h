#pragma once

#include <iostream>

using namespace std;

//class XMFLOAT3 {
//public:
//	XMFLOAT3() = default;
//	XMFLOAT3(float x, float y, float z);
//	~XMFLOAT3() = default;
//public:
//	float x{ 0.f };
//	float y{ 0.f };
//	float z{ 0.f };
//public:
//	XMFLOAT3 operator+(const XMFLOAT3& rhs) {
//		this->x += rhs.x;
//		this->y += rhs.y;
//		this->z += rhs.z;
//		return *this;
//	}
//};

class Point {
public:
	Point() = default;
	Point(int x, int y);
	~Point() = default;
public:
	int x{ 0 };
	int y{ 0 };
public:
	Point operator+(const Point& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
};

// XMFLOAT3 front(0.f, 0.f, 1.f);
// XMFLOAT3 back(0.f, 0.f, -1.f);
// XMFLOAT3 left(-1.f, 0.f, 0.f);
// XMFLOAT3 right(1.f, 0.f, 0.f);
// XMFLOAT3 top(0.f, 1.f, 0.f);
// XMFLOAT3 bottom(0.f, -1.f, 0.f);

Point pLeft(-1, 0);
Point pRight(1, 0);
Point top(0, 1);
Point bottom(0, -1);

// void PrintXMFLOAT3(const char* title, const XMFLOAT3& xmfloat3);
void PrintPoint(const char* title, const Point& point);