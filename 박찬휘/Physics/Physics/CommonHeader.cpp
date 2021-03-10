#include "CommonHeader.h"

//XMFLOAT3::XMFLOAT3(float x, float y, float z)
//{
//	this->x = x;
//	this->y = y;
//	this->z = z;
//}

Point::Point(int x, int y)
{
	this->x = x;
	this->y = y;
}

//void PrintXMFLOAT3(const char* title, const XMFLOAT3& xmfloat3)
//{
//	cout << title << "(" << xmfloat3.x << ", " << xmfloat3.y << ", " << xmfloat3.z << ")" << endl;
//}

void PrintPoint(const char* title, const Point& point)
{
	cout << title << "(" << point.x << ", " << point.y << ")" << endl;
}


