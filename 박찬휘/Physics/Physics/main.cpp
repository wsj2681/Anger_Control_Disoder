#include <conio.h>
#include <Windows.h>
#include "CommonHeader.h"
#include "Physics.h"
#include "Object.h"

char GetKey();

int main() {
	unsigned int num{ 0 };
	/*XMFLOAT3 size1(3.f, 3.f, 3.f);
	XMFLOAT3 pos1(-5.f, 3.f, 5.f);
	XMFLOAT3 size2(5.f, 5.f, 5.f);
	XMFLOAT3 pos2(6.f, 7.f, 0.f);
	XMFLOAT3 size3(7.f, 7.f, 7.f);
	XMFLOAT3 pos3(3.f, 9.f, -5.f);*/

	Point size1(3, 3);
	Point pos1(-5, 3);
	Point size2(5, 5);
	Point pos2(6, 7);
	Point size3(7, 7);
	Point pos3(3, 9);

	Object obj1(++num, size1, pos1);
	FixedObject obj2(++num, size2, pos2);
	MovableObject obj3(++num, size3, pos3);

	char c;
	while (true)
	{
		c = GetKey();
		switch (c)
		{
		/*case 'w':
		case 'W':
			obj3.Move(front);
			cout << "Move(front)" << endl;
			break;
		case 'a':
		case 'A':
			obj3.Move(left);
			cout << "Move(left)" << endl;
			break;
		case 's':
		case 'S':
			obj3.Move(back);
			cout << "Move(back)" << endl;
			break;
		case 'd':
		case 'D':
			obj3.Move(right);
			cout << "Move(right)" << endl;
			break;
		case 'r':
		case 'R':
			obj3.Move(top);
			cout << "Move(top)" << endl;
			break;
		case 'f':
		case 'F':
			obj3.Move(bottom);
			cout << "Move(bottom)" << endl;
			break;*/

		case 'w':
		case 'W':
			obj3.Move(top);
			cout << "Move(top)" << endl;
			break;
		case 'a':
		case 'A':
			obj3.Move(bottom);
			cout << "Move(bottom)" << endl;
			break;
		case 's':
		case 'S':
			obj3.Move(pLeft);
			cout << "Move(left)" << endl;
			break;
		case 'd':
		case 'D':
			obj3.Move(pRight);
			cout << "Move(right)" << endl;
			break;
		case 'q':
		case 'Q':
			exit(0);
			break;
		}
		// Render
		//obj1.Render();
		//obj2.Render();
		obj3.Render();

		// Sleep
		Sleep(100);
	}
	return 0;
}

char GetKey()
{
	if (_kbhit())
		return _getch();
	return '\0';
}