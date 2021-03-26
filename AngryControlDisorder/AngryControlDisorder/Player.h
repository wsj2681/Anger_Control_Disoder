#pragma once
#include "Object.h"

class Camera;

class Player : public Object
{
public:

	Player() = default;
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
	virtual ~Player();

protected:

	XMFLOAT3 position{ 0.f, 0.f, 0.f };
	XMFLOAT3 right{ 1.f, 0.f, 0.f };
	XMFLOAT3 up{ 0.f, 1.f, 0.f };
	XMFLOAT3 look{ 0.f, 0.f, 1.f };
	XMFLOAT3 scale{ 1.f, 1.f, 1.f };

	float pitch = 0.f;
	float roll = 0.f;
	float yaw = 0.f;

	XMFLOAT3 velocity{ 0.f, 0.f, 0.f };
	XMFLOAT3 gravity{ 0.f, 0.f, 0.f };
	float maxVelocityXZ = 0.f;
	float maxVelocityY = 0.f;
	float friction = 0.f;

	LPVOID playerUpdatedContext = nullptr;
	LPVOID cameraUpdatedContext = nullptr;

	Camera* camera = nullptr;

public:

	virtual void Update(float fTimeElapsed);
	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera = nullptr);

	void Move(ULONG direction, float distance, bool velocity = false);
	void Move(const XMFLOAT3& shift, bool velocity = false);
	void Move(float xOffset = 0.f, float yOffset = 0.f, float zOffset = 0.f);
	void Rotate(float x, float y, float z);

	virtual void OnPlayerUpdateCallback(float fTimeElapsed);
	void SetPlayerUpdatedContext(LPVOID pContext);

	virtual void OnCameraUpdateCallback(float fTimeElapsed);
	void SetCameraUpdatedContext(LPVOID pContext);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	Camera* GetCamera();
	void SetCamera(Camera* camera);
	Camera* OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode);
	virtual Camera* ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed);

	const XMFLOAT3& GetRightVector();
	void SetRightVector(const XMFLOAT3& right);
	void SetRightVector(float x, float y, float z);

	const XMFLOAT3& GetUpVector();
	void GetUpVector(const XMFLOAT3& up);
	void GetUpVector(float x, float y, float z);
	
	const XMFLOAT3& GetLookVector();
	void SetLookVector(const XMFLOAT3& look);
	void SetLookVector(float x, float y, float z);

	const XMFLOAT3& GetScale();
	void SetScale(const XMFLOAT3& scale);

	const float& GetPitch();
	void SetPitch(float pitch);

	const float& GetRoll();
	void SetRoll(float roll);

	const float& GetYaw();
	void SetYaw(float yaw);

	const float& GetFriction();
	void SetFriction(float friction);
	
	const XMFLOAT3& GetGravity();
	void SetGravity(const XMFLOAT3& gravity);

	const XMFLOAT3& GetVelocity();
	void SetMaxVelocityXZ(float maxVelocity);
	void SetMaxVelocityY(float maxVelocity);
	void SetVelocity(const XMFLOAT3& velocity);

	const XMFLOAT3& GetPosition();
	void SetPoisition(const XMFLOAT3& position);

};

