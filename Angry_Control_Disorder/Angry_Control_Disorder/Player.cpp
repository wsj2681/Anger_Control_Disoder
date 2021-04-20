#include "framework.h"
#include "Player.h"
#include "AnimationController.h"
#include "AnimationSet.h"
#include "CallBack.h"
#include "ModelInfo.h"
#include "Shader.h"

void Player::OnPrepareRender()
{
	m_xmf4x4ToParent._11 = m_xmf3Right.x; m_xmf4x4ToParent._12 = m_xmf3Right.y; m_xmf4x4ToParent._13 = m_xmf3Right.z;
	m_xmf4x4ToParent._21 = m_xmf3Up.x; m_xmf4x4ToParent._22 = m_xmf3Up.y; m_xmf4x4ToParent._23 = m_xmf3Up.z;
	m_xmf4x4ToParent._31 = m_xmf3Look.x; m_xmf4x4ToParent._32 = m_xmf3Look.y; m_xmf4x4ToParent._33 = m_xmf3Look.z;
	m_xmf4x4ToParent._41 = m_xmf3Position.x; m_xmf4x4ToParent._42 = m_xmf3Position.y; m_xmf4x4ToParent._43 = m_xmf3Position.z;

	m_xmf4x4ToParent = Matrix4x4::Multiply(XMMatrixScaling(m_xmf3Scale.x, m_xmf3Scale.y, m_xmf3Scale.z), m_xmf4x4ToParent);
}

void Player::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	DWORD nCameraMode = (pCamera) ? pCamera->GetMode() : 0x00;
	if (nCameraMode == THIRD_PERSON_CAMERA) Object::Render(pd3dCommandList, pCamera);
}

void Player::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (m_pCamera) m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void Player::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void Player::OnPlayerUpdateCallback(float fTimeElapsed)
{
}

void Player::SetPlayerUpdatedContext(LPVOID pContext)
{
}

void Player::OnCameraUpdateCallback(float fTimeElapsed)
{
}

void Player::SetCameraUpdatedContext(LPVOID pContext)
{
}

void Player::ReleaseShaderVariables()
{
	if (m_pCamera) m_pCamera->ReleaseShaderVariables();
}

void Player::Move(DWORD dwDirection, float fDistance, bool bVelocity)
{
}

void Player::Move(const XMFLOAT3& xmf3Shift, bool bVelocity)
{
}

void Player::Move(float fxOffset, float fyOffset, float fzOffset)
{
}

void Player::Rotate(float x, float y, float z)
{
}

void Player::Update(float fTimeElapsed)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, XMFLOAT3());
	float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.z * m_xmf3Velocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ;
	if (fLength > m_fMaxVelocityXZ)
	{
		m_xmf3Velocity.x *= (fMaxVelocityXZ / fLength);
		m_xmf3Velocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_fMaxVelocityY;
	fLength = sqrtf(m_xmf3Velocity.y * m_xmf3Velocity.y);
	if (fLength > m_fMaxVelocityY) m_xmf3Velocity.y *= (fMaxVelocityY / fLength);

	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(m_xmf3Velocity, fTimeElapsed, false);
	Move(xmf3Velocity, false);

	if (m_pPlayerUpdatedContext) OnPlayerUpdateCallback(fTimeElapsed);

	DWORD nCurrentCameraMode = m_pCamera->GetMode();
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->Update(head->GetPosition(), fTimeElapsed);
	if (m_pCameraUpdatedContext) OnCameraUpdateCallback(fTimeElapsed);
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->SetLookAt(head->GetPosition());
	m_pCamera->RegenerateViewMatrix();

	fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = (m_fFriction * fTimeElapsed);
	if (fDeceleration > fLength) fDeceleration = fLength;
	m_xmf3Velocity = XMFLOAT3();//Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));
}

Camera* Player::OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode)
{
	Camera* pNewCamera = NULL;
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		pNewCamera = new FirstPersonCamera(m_pCamera);
		break;
	case THIRD_PERSON_CAMERA:
		pNewCamera = new ThirdPersonCamera(m_pCamera);
		break;
	case SPACESHIP_CAMERA:
		pNewCamera = new SpaceShipCamera(m_pCamera);
		break;
	}
	if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_xmf3Right = Vector3::Normalize(XMFLOAT3(m_xmf3Right.x, 0.0f, m_xmf3Right.z));
		m_xmf3Up = Vector3::Normalize(XMFLOAT3(0.0f, 1.0f, 0.0f));
		m_xmf3Look = Vector3::Normalize(XMFLOAT3(m_xmf3Look.x, 0.0f, m_xmf3Look.z));

		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		m_fYaw = Vector3::Angle(XMFLOAT3(0.0f, 0.0f, 1.0f), m_xmf3Look);
		if (m_xmf3Look.x < 0.0f) m_fYaw = -m_fYaw;
	}
	else if ((nNewCameraMode == SPACESHIP_CAMERA) && m_pCamera)
	{
		m_xmf3Right = m_pCamera->GetRightVector();
		m_xmf3Up = m_pCamera->GetUpVector();
		m_xmf3Look = m_pCamera->GetLookVector();
	}

	if (pNewCamera)
	{
		pNewCamera->SetMode(nNewCameraMode);
		pNewCamera->SetPlayer(this);
	}

	if (m_pCamera) delete m_pCamera;

	return(pNewCamera);
}


Camera* Player::ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed)
{
	return nullptr;
}

Camera* Player::GetCamera()
{
	return this->m_pCamera;
}

void Player::SetPosition(const XMFLOAT3& position)
{
	Move(position, false);
}

const XMFLOAT3& Player::GetPosition()
{
	return this->m_xmf3Position;
}

const XMFLOAT3& Player::GetLookVector()
{
	return this->m_xmf3Look;
}

const XMFLOAT3& Player::GetUpVector()
{
	return this->m_xmf3Up;
}

const XMFLOAT3& Player::GetRightVector()
{
	return this->m_xmf3Right;
}

void Player::SetFriction(float fFriction)
{
	this->m_fFriction = fFriction;
}

const float& Player::GetFriction()
{
	return this->m_fFriction;
}

void Player::SetGravity(const XMFLOAT3& xmf3Gravity)
{
	m_xmf3Gravity = xmf3Gravity;
}

void Player::SetMaxVelocityXZ(float fMaxVelocity)
{
	m_fMaxVelocityXZ = fMaxVelocity;
}

void Player::SetMaxVelocityY(float fMaxVelocity)
{
	m_fMaxVelocityY = fMaxVelocity;
}

void Player::SetVelocity(const XMFLOAT3& xmf3Velocity)
{
	m_xmf3Velocity = xmf3Velocity;
}

void Player::SetVelocity(const float& x, const float& y, const float& z)
{
	SetVelocity(XMFLOAT3(x, y, z));
}

const XMFLOAT3& Player::GetVelocity()
{
	return this->m_xmf3Velocity;
}

void Player::SetScale(const XMFLOAT3& xmf3Scale)
{
	m_xmf3Scale = xmf3Scale;
}

void Player::SetScale(const float& x, const float& y, const float& z)
{
	SetScale(XMFLOAT3(x, y, z));
}

const float& Player::GetYaw()
{
	return m_fYaw;
}

const float& Player::GetPitch()
{
	return m_fPitch;
}

const float& Player::GetRoll()
{
	return m_fRoll;
}

