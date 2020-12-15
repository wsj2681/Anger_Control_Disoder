#pragma once

#define DIR_FORWARD 0x01
#define DIR_BACKWARD 0x02
#define DIR_LEFT 0x04
#define DIR_RIGHT 0x08
#define DIR_UP 0x10
#define DIR_DOWN 0x20

#include "GameObject.h"
#include "Camera.h"

class CPlayer :public CGameObject {



	XMFLOAT3 m_xmf3Position{ XMFLOAT3(0.f, 0.f, 0.f) };
	XMFLOAT3 m_xmf3Right{ XMFLOAT3(1.f, 0.f, 0.f) };
	XMFLOAT3 m_xmf3Up{ XMFLOAT3(0.f, 1.f, 0.f) };
	XMFLOAT3 m_xmf3Look{ XMFLOAT3(0.f, 0.f, 1.f) };

	float m_fPitch{ 0.f };
	float m_fRoll{ 0.f };
	float m_fYaw{ 0.f };


	XMFLOAT3 m_xmf3Velocity{ XMFLOAT3(0.f, 0.f, 0.f) };
	float m_fMaxVelocityXZ{ 0.f };
	float m_fMaxVelocityY{ 0.f };

	XMFLOAT3 m_xmf3Gravity{ XMFLOAT3(0.f, 0.f, 0.f) };

	float m_fFriction{ 0.f };

	LPVOID m_pPlayerUpdatedContext{ nullptr };
	LPVOID m_pCameraUpdatedContext{ nullptr };


public:
	CCamera* m_pCamera{ nullptr };
	XMFLOAT4X4 m_xmf4x4World{ Matrix4x4::Identity() };
	CPlayer() {}
	virtual ~CPlayer() {
		ReleaseShaderVariables();

		if (m_pCamera)
			delete m_pCamera;
	}

	XMFLOAT3 GetPosition() const { return(m_xmf3Position); }
	XMFLOAT3 GetLookVector() const { return(m_xmf3Look); }
	XMFLOAT3 GetUpVector() const { return(m_xmf3Up); }
	XMFLOAT3 GetRightVector() const { return(m_xmf3Right); }
	void SetFriction(float fFriction) { m_fFriction = fFriction; }
	void SetGravity(const XMFLOAT3& xmf3Gravity) { m_xmf3Gravity = xmf3Gravity; }
	void SetMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; }
	void SetMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; }
	void SetVelocity(const XMFLOAT3& xmf3Velocity) { m_xmf3Velocity = xmf3Velocity; }

	void SetPosition(const XMFLOAT3& xmf3Position) {
		Move(XMFLOAT3(xmf3Position.x - m_xmf3Position.x,
			xmf3Position.y - m_xmf3Position.y,
			xmf3Position.z - m_xmf3Position.z),
			false);
	}
	XMFLOAT3 GetVelocity() const { return(m_xmf3Velocity); }
	float GetYaw()const { return(m_fYaw); }
	float GetPitch()const { return(m_fPitch); }
	float GetRoll() const { return(m_fRoll); }
	CCamera* GetCamera() const { return(m_pCamera); }
	void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }

	void Move(ULONG nDirection, float fDistance, bool bVelocity = false);
	void Move(const XMFLOAT3& xmf3Shift, bool bVelocity = false);
	void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);

	void Rotate(float x, float y, float z);
	void Update(float fTimeElapsed);

	virtual void OnPlayerUpdateCallback(float fTimeElapsed) { }
	void SetPlayerUpdatedContext(LPVOID pContext) { m_pPlayerUpdatedContext = pContext; }

	virtual void OnCameraUpdateCallback(float fTimeElapsed) { }
	void SetCameraUpdatedContext(LPVOID pContext) { m_pCameraUpdatedContext = pContext; }
	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	CCamera* OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode);
	virtual CCamera* ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed) {
		return nullptr;
	}

	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = nullptr);
};

class CAirplanePlayer : public CPlayer
{
public:
	CAirplanePlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
		ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual ~CAirplanePlayer();
	virtual CCamera* ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed);
	virtual void OnPrepareRender();
};