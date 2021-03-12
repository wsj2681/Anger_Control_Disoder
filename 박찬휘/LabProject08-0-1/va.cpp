#include "stdafx.h"

using namespace std;

// ���� F = ma���� a = F/m�� ���ӵ� ����
// �ӵ�(i) = ���ӵ�(a) * �ð�(t)
// ��ġ(p) = (�ӵ�)i * �ð�(t)

//ȸ�� �������� ��� T = iw���� w = T/i�� �����ӵ� ����
// ���ӵ�(an) = �����ӵ�(w) * �ð�(t)
// ���� ȸ�� ����(���⺤�� ����) =  ���ӵ�(an) * �ð�(t)

float mass;	// ����(M)
XMFLOAT4X4 inertiaTensor;	// ȸ�� ����(it)
vector linVel;	// �� �ӵ�
vector angVel;	// �� �ӵ�
vector center;	// ���� �߽� (��ġ ǥ��)
vector right, up, forward;
BoundingBox boundingBox;	// �ٿ�� �ڽ�
XMFLOAT4X4 state;	// ��ġ �̵�, ȸ��, ������ ���� ����
					// ����� ���� m00 m01 m02
					// ���� ����   m10 m11 m12
					// ���� ����   m20 m21 m22
					// ��ġ        m30 m31 m32

float airRes;			// ���� ����
float friction;			// ������
float restitution;		// ź�����
float kineticMotion;	// �������
bool sleep;				// sleep ����
bool movable;			// ������ �� ����
bool collidable;		// �浹�� �� ����