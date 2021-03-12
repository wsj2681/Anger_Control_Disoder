#include "stdafx.h"

using namespace std;

// 질량 F = ma에서 a = F/m로 가속도 구함
// 속도(i) = 가속도(a) * 시간(t)
// 위치(p) = (속도)i * 시간(t)

//회전 동역학의 요소 T = iw에서 w = T/i로 각가속도 구함
// 각속도(an) = 각가속도(w) * 시간(t)
// 현재 회전 상태(방향벡터 상태) =  각속도(an) * 시간(t)

float mass;	// 질량(M)
XMFLOAT4X4 inertiaTensor;	// 회전 관성(it)
vector linVel;	// 선 속도
vector angVel;	// 각 속도
vector center;	// 무게 중심 (위치 표현)
vector right, up, forward;
BoundingBox boundingBox;	// 바운딩 박스
XMFLOAT4X4 state;	// 위치 이동, 회전, 렌더링 고유 상태
					// 우방향 벡터 m00 m01 m02
					// 상향 벡터   m10 m11 m12
					// 전방 벡터   m20 m21 m22
					// 위치        m30 m31 m32

float airRes;			// 공기 저항
float friction;			// 마찰력
float restitution;		// 탄성계수
float kineticMotion;	// 운동에너지
bool sleep;				// sleep 상태
bool movable;			// 움직일 수 있음
bool collidable;		// 충돌할 수 있음