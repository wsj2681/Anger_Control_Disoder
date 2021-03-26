#include "framework.h"
#include "Player.h"
#include "Camera.h"

Player::~Player()
{
	ReleaseShaderVariables();

	DELETE_CLASS(camera);
}

void Player::Update(float fTimeElapsed)
{
	velocity = Vector3::Add(velocity, XMFLOAT3());
	float fLength = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	float fMaxVelocityXZ = maxVelocityXZ;
	if (fLength > maxVelocityXZ)
	{
		velocity.x *= (fMaxVelocityXZ / fLength);
		velocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = maxVelocityY;
	fLength = sqrtf(velocity.y * velocity.y);
	if (fLength > maxVelocityY)
	{
		velocity.y *= (fMaxVelocityY / fLength);
	}
	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(velocity, fTimeElapsed, false);
	Move(xmf3Velocity, false);

	if (playerUpdatedContext) OnPlayerUpdateCallback(fTimeElapsed);

	DWORD nCurrentCameraMode = camera->GetMode();
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA)
	{
		camera->Update(position, fTimeElapsed);
	}
	if (cameraUpdatedContext)
	{
		OnCameraUpdateCallback(fTimeElapsed);
	}
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA)
	{
		camera->SetLookAt(position);
	}

	camera->RegenerateViewMatrix();

	fLength = Vector3::Length(velocity);

	float fDeceleration = (friction * fTimeElapsed);
	if (fDeceleration > fLength)
	{
		fDeceleration = fLength;
	}
	
	velocity = XMFLOAT3();
}

void Player::OnPrepareRender()
{ 
	toParent._11 = right.x; toParent._12 = right.y; toParent._13 = right.z;
	toParent._21 = up.x; toParent._22 = up.y; toParent._23 = up.z;
	toParent._31 = look.x; toParent._32 = look.y; toParent._33 = look.z;
	toParent._41 = position.x; toParent._42 = position.y; toParent._43 = position.z;

	toParent = Matrix4x4::Multiply(XMMatrixScaling(scale.x, scale.y, scale.z), toParent);
}

void Player::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	DWORD cameraMode = (pCamera) ? pCamera->GetMode() : 0x00;
	if (cameraMode == THIRD_PERSON_CAMERA)
	{
		Object::Render(pd3dCommandList, pCamera);
	}
}

void Player::Move(ULONG direction, float distance, bool velocity)
{
	if (direction)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (direction & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, look, distance);
		if (direction & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, look, -distance);
		if (direction & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, right, distance);
		if (direction & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, right, -distance);
		if (direction & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, up, distance);
		if (direction & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, up, -distance);

		Move(xmf3Shift, velocity);
	}
}

void Player::Move(const XMFLOAT3& shift, bool velocity)
{
	if (velocity)
	{
		this->velocity = Vector3::Add(this->velocity, shift);
	}
	else
	{
		position = Vector3::Add(position, shift);
		camera->Move(shift);
	}
}

void Player::Move(float xOffset, float yOffset, float zOffset)
{
}

void Player::Rotate(float x, float y, float z)
{
	DWORD nCurrentCameraMode = camera->GetMode();
	if ((nCurrentCameraMode == FIRST_PERSON_CAMERA) || (nCurrentCameraMode == THIRD_PERSON_CAMERA))
	{
		if (x != 0.0f)
		{
			pitch += x;
			if (pitch > +89.0f) 
			{
				x -= (pitch - 89.0f);
				pitch = +89.0f;
			}
			if (pitch < -89.0f)
			{ 
				x -= (pitch + 89.0f);
				pitch = -89.0f;
			}
		}
		if (y != 0.0f)
		{
			yaw += y;
			if (yaw > 360.0f)
			{
				yaw -= 360.0f;
			}
			if (yaw < 0.0f)
			{
				yaw += 360.0f;
			}
		}
		if (z != 0.0f)
		{
			roll += z;
			if (roll > +20.0f) 
			{ 
				z -= (roll - 20.0f); 
				roll = +20.0f; 
			}
			if (roll < -20.0f) 
			{ 
				z -= (roll + 20.0f);
				roll = -20.0f;
			}
		}
		camera->Rotate(x, y, z);

		if (y != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&up), XMConvertToRadians(y));
			look = Vector3::TransformNormal(look, xmmtxRotate);
			right = Vector3::TransformNormal(right, xmmtxRotate);
		}
	}
	else if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		camera->Rotate(x, y, z);
		if (x != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&right), XMConvertToRadians(x));
			look = Vector3::TransformNormal(look, xmmtxRotate);
			up = Vector3::TransformNormal(up, xmmtxRotate);
		}
		if (y != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&up), XMConvertToRadians(y));
			look = Vector3::TransformNormal(look, xmmtxRotate);
			right = Vector3::TransformNormal(right, xmmtxRotate);
		}
		if (z != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&look), XMConvertToRadians(z));
			up = Vector3::TransformNormal(up, xmmtxRotate);
			right = Vector3::TransformNormal(right, xmmtxRotate);
		}
	}

	look = Vector3::Normalize(look);
	right = Vector3::CrossProduct(up, look, true);
	up = Vector3::CrossProduct(look, right, true);
}

void Player::OnPlayerUpdateCallback(float fTimeElapsed)
{
}

void Player::SetPlayerUpdatedContext(LPVOID pContext)
{
	playerUpdatedContext = pContext;
}

void Player::OnCameraUpdateCallback(float fTimeElapsed)
{
}

void Player::SetCameraUpdatedContext(LPVOID pContext)
{
	cameraUpdatedContext = pContext;
}

void Player::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (camera) camera->CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void Player::ReleaseShaderVariables()
{
	if (camera) camera->ReleaseShaderVariables();
}

void Player::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

Camera* Player::GetCamera()
{
	return this->camera;
}

void Player::SetCamera(Camera* camera)
{
	this->camera = camera;
}

Camera* Player::OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode)
{
	return nullptr;
}

Camera* Player::ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed)
{
	return nullptr;
}

const XMFLOAT3& Player::GetRightVector()
{
	return right;
}

void Player::SetRightVector(const XMFLOAT3& right)
{
	this->right = right;
}

void Player::SetRightVector(float x, float y, float z)
{
	right.x = x;
	right.y = y;
	right.z = z;
}

const XMFLOAT3& Player::GetUpVector()
{
	return up;
}

void Player::GetUpVector(const XMFLOAT3& up)
{
	this->up = up;
}

void Player::GetUpVector(float x, float y, float z)
{
	up.x = x;
	up.y = y;
	up.z = z;
}

const XMFLOAT3& Player::GetLookVector()
{
	return look;
}

void Player::SetLookVector(const XMFLOAT3& look)
{
	this->look = look;
}

void Player::SetLookVector(float x, float y, float z)
{
	look.x = x;
	look.y = y;
	look.z = z;
}

const XMFLOAT3& Player::GetScale()
{
	return XMFLOAT3();
}

void Player::SetScale(const XMFLOAT3& scale)
{
	this->scale = scale;
}

const float& Player::GetPitch()
{
	return pitch;
}

void Player::SetPitch(float pitch)
{
	this->pitch = pitch;
}

const float& Player::GetRoll()
{
	return roll;
}

void Player::SetRoll(float roll)
{
	this->roll = roll;
}

const float& Player::GetYaw()
{
	return yaw;
}

void Player::SetYaw(float yaw)
{
	this->yaw = yaw;
}

const float& Player::GetFriction()
{
	return friction;
}

void Player::SetFriction(float friction)
{
	this->friction = friction;
}

const XMFLOAT3& Player::GetGravity()
{
	return gravity;
}

void Player::SetGravity(const XMFLOAT3& gravity)
{
	this->gravity = gravity;
}

const XMFLOAT3& Player::GetVelocity()
{
	return velocity;
}

void Player::SetMaxVelocityXZ(float maxVelocity)
{
	this->maxVelocityXZ = maxVelocity;
}

void Player::SetMaxVelocityY(float maxVelocity)
{
	this->maxVelocityY = maxVelocity;
}

void Player::SetVelocity(const XMFLOAT3& velocity)
{
	this->velocity = velocity;
}

const XMFLOAT3& Player::GetPosition()
{
	return position;
}

void Player::SetPoisition(const XMFLOAT3& position)
{
	this->position = position;
}