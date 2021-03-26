#include "framework.h"
#include "SpaceShipCamera.h"

SpaceShipCamera::SpaceShipCamera(Camera* camera) : Camera(camera)
{
	cameraMode = SPACESHIP_CAMERA;
}

SpaceShipCamera::~SpaceShipCamera()
{
}

void SpaceShipCamera::Rotate(float fPitch, float fYaw, float fRoll)
{
	if (player && (fPitch != 0.0f))
	{
		XMFLOAT3 xmf3Right = player->GetRightVector();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Right), XMConvertToRadians(fPitch));
		right = Vector3::TransformNormal(right, xmmtxRotate);
		up = Vector3::TransformNormal(up, xmmtxRotate);
		look = Vector3::TransformNormal(look, xmmtxRotate);
		position = Vector3::Subtract(position, player->GetPosition());
		position = Vector3::TransformCoord(position, xmmtxRotate);
		position = Vector3::Add(position, player->GetPosition());
	}
	if (player && (fYaw != 0.0f))
	{
		XMFLOAT3 xmf3Up = player->GetUpVector();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Up), XMConvertToRadians(fYaw));
		right = Vector3::TransformNormal(right, xmmtxRotate);
		up = Vector3::TransformNormal(up, xmmtxRotate);
		look = Vector3::TransformNormal(look, xmmtxRotate);
		position = Vector3::Subtract(position, player->GetPosition());
		position = Vector3::TransformCoord(position, xmmtxRotate);
		position = Vector3::Add(position, player->GetPosition());
	}
	if (player && (fRoll != 0.0f))
	{
		XMFLOAT3 xmf3Look = player->GetLookVector();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Look), XMConvertToRadians(fRoll));
		right = Vector3::TransformNormal(right, xmmtxRotate);
		up = Vector3::TransformNormal(up, xmmtxRotate);
		look = Vector3::TransformNormal(look, xmmtxRotate);
		position = Vector3::Subtract(position, player->GetPosition());
		position = Vector3::TransformCoord(position, xmmtxRotate);
		position = Vector3::Add(position, player->GetPosition());
	}
}
