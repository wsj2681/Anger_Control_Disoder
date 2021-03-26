#include "framework.h"
#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(Camera* camera)
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

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::Rotate(float fPitch, float fYaw, float fRoll)
{
	if (fPitch != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&right), XMConvertToRadians(fPitch));
		look = Vector3::TransformNormal(look, xmmtxRotate);
		up = Vector3::TransformNormal(up, xmmtxRotate);
		right = Vector3::TransformNormal(right, xmmtxRotate);
	}
	if (player && (fYaw != 0.0f))
	{
		XMFLOAT3 xmf3Up = player->GetUpVector();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Up), XMConvertToRadians(fYaw));
		look = Vector3::TransformNormal(look, xmmtxRotate);
		up = Vector3::TransformNormal(up, xmmtxRotate);
		right = Vector3::TransformNormal(right, xmmtxRotate);
	}
	if (player && (fRoll != 0.0f))
	{
		XMFLOAT3 xmf3Look = player->GetLookVector();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Look), XMConvertToRadians(fRoll));
		position = Vector3::Subtract(position, player->GetPosition());
		position = Vector3::TransformCoord(position, xmmtxRotate);
		position = Vector3::Add(position, player->GetPosition());
		look = Vector3::TransformNormal(look, xmmtxRotate);
		up = Vector3::TransformNormal(up, xmmtxRotate);
		right = Vector3::TransformNormal(right, xmmtxRotate);
	}
}
