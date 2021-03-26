#include "framework.h"
#include "ThirdPersonCamera.h"

ThirdPersonCamera::ThirdPersonCamera(Camera* camera)
{
	cameraMode = FIRST_PERSON_CAMERA;

	if (camera)
	{
		if (camera->GetMode() == SPACESHIP_CAMERA)
		{
			up = XMFLOAT3(0.f, 1.f, 0.f);
			right.y = 0.f;
			look.y = 0.f;
			right = Vector3::Normalize(right);
			look = Vector3::Normalize(look);
		}
	}
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::Rotate(float fPitch, float fYaw, float fRoll)
{
}

void ThirdPersonCamera::Update(const XMFLOAT3& lookAt, float eTime)
{
	if (player->head)
	{
		XMFLOAT4X4 xmf4x4Rotate = Matrix4x4::Identity();
		XMFLOAT3 xmf3Right = player/*->head->GetRight();*/->GetRightVector();
		XMFLOAT3 xmf3Up = player->/*head->GetUp();*/GetUpVector();
		XMFLOAT3 xmf3Look = player->/*head->GetLook();*/GetLookVector();
		xmf4x4Rotate._11 = xmf3Right.x; xmf4x4Rotate._21 = xmf3Up.x; xmf4x4Rotate._31 = xmf3Look.x;
		xmf4x4Rotate._12 = xmf3Right.y; xmf4x4Rotate._22 = xmf3Up.y; xmf4x4Rotate._32 = xmf3Look.y;
		xmf4x4Rotate._13 = xmf3Right.z; xmf4x4Rotate._23 = xmf3Up.z; xmf4x4Rotate._33 = xmf3Look.z;

		XMFLOAT3 xmf3Offset = Vector3::TransformCoord(offset, xmf4x4Rotate);
		XMFLOAT3 xmf3Position = Vector3::Add(player->GetPosition(), xmf3Offset);
		XMFLOAT3 xmf3Direction = Vector3::Subtract(xmf3Position, position);
		float fLength = Vector3::Length(xmf3Direction);
		xmf3Direction = Vector3::Normalize(xmf3Direction);
		
		float fTimeLagScale = (timeLag) ? eTime * (1.0f / timeLag) : 1.0f; //카메라 회전 지연시간
		float fDistance = fLength * fTimeLagScale;
		
		if (fDistance > fLength)
		{
			fDistance = fLength;
		}
		if (fLength < 0.01f) 
		{
			fDistance = fLength;
		}
		if (fDistance > 0)
		{
			position = Vector3::Add(position, xmf3Direction, fDistance);
			//m_xmf3Position = XMFLOAT3(m_xmf3Position.x, m_pPlayer->GetPosition().y + 22.f, m_xmf3Position.z  + 3.f);
			SetLookAt(lookAt);
		}
	}
}

void ThirdPersonCamera::SetLookAt(const XMFLOAT3& lookAt)
{
	XMFLOAT4X4 mtxLookAt = Matrix4x4::LookAtLH(position, lookAt, player->GetUpVector());
	right = XMFLOAT3(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31);
	up = XMFLOAT3(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32);
	look = XMFLOAT3(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33);
}
