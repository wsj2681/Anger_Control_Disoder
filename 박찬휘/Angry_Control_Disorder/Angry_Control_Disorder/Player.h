#pragma once
#include "Object.h"
#include "Camera.h"

enum class Animation
{
	ANIMATION_IDLE,
	ANIMATION_ATTACK_LOOP,
	ANIMATION_MOVE_FORWARD,
	ANIMATION_MOVE_BACKWARD,
	ANIMATION_MOVE_LEFT,
	ANIMATION_MOVE_RIGHT,
	ANIMATION_ATTACK_KICK,
	ANIMATION_CEREMONY,
	ANIMATION_GUARD_LEFT_HEAD,
	ANIMATION_GUARD_RIGHT_HEAD,
	ANIMATION_GUARD_BODY
};

class AnimationCallbackHandler;

class Player : public Object
{
public:

	Player() = default;
	virtual ~Player() = default;

protected:

	XMFLOAT3 m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3 m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3 m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);
			 
	XMFLOAT3 m_xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
			 
	float m_fPitch = 0.0f;
	float m_fYaw = 0.0f;
	float m_fRoll = 0.0f;
			 
	XMFLOAT3 m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float m_fMaxVelocityXZ = 0.0f;
	float m_fMaxVelocityY = 0.0f;
	float m_fFriction = 0.0f;
			 
	LPVOID m_pPlayerUpdatedContext = nullptr;
	LPVOID m_pCameraUpdatedContext = nullptr;

	Camera* m_pCamera = nullptr;

public: /* Method */

	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera = nullptr);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual void OnPlayerUpdateCallback(float fTimeElapsed);
	void SetPlayerUpdatedContext(LPVOID pContext);

	virtual void OnCameraUpdateCallback(float fTimeElapsed);
	void SetCameraUpdatedContext(LPVOID pContext);

	void Move(DWORD dwDirection, float fDistance, bool bVelocity = false);
	void Move(const XMFLOAT3& xmf3Shift, bool bVelocity = false);
	void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);
	void Rotate(float x, float y, float z);

public: /* Camera Method */

	virtual void Update(float fTimeElapsed);
	Camera* OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode);
	virtual Camera* ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed);
	Camera* GetCamera();
public: /* Get Set */

	void SetPosition(const XMFLOAT3& position);
	const XMFLOAT3& GetPosition();

	const XMFLOAT3& GetLookVector();
	const XMFLOAT3& GetUpVector();
	const XMFLOAT3& GetRightVector();

	void SetFriction(float fFriction);
	const float& GetFriction();

	void SetGravity(const XMFLOAT3& xmf3Gravity);
	
	void SetMaxVelocityXZ(float fMaxVelocity);
	void SetMaxVelocityY(float fMaxVelocity);
	
	void SetVelocity(const XMFLOAT3& xmf3Velocity);
	void SetVelocity(const float& x, const float& y, const float& z);
	const XMFLOAT3& GetVelocity();

	void SetScale(const XMFLOAT3& xmf3Scale);
	void SetScale(const float& x, const float& y, const float& z);

	const float& GetYaw();
	const float& GetPitch();
	const float& GetRoll();



};

