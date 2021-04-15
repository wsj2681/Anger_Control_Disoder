#pragma once

#define ASPECT_RATIO				(float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT))

#define FIRST_PERSON_CAMERA			0x01
#define SPACESHIP_CAMERA			0x02
#define THIRD_PERSON_CAMERA			0x03

struct VS_CB_CAMERA_INFO
{
	XMFLOAT4X4						m_xmf4x4View;
	XMFLOAT4X4						m_xmf4x4Projection;
	XMFLOAT3						m_xmf3Position;
};

class Player;

class Camera
{
public:

	Camera() = default;
	Camera(Camera* pCamera);
	virtual ~Camera() = default;

protected:

	XMFLOAT3 m_xmf3Position{ 0.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Right{ 0.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Up{ 0.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Look{ 0.f, 0.f, 0.f };

	float m_fPitch = 0.f;
	float m_fRoll = 0.f;
	float m_fYaw = 0.f;

	DWORD m_nMode = 0x00;

	XMFLOAT3 m_xmf3LookAtWorld{ 0.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Offset{ 0.f, 0.f, 0.f };
	float m_fTimeLag = 0.f;

	XMFLOAT4X4 m_xmf4x4View = Matrix4x4::Identity();
	XMFLOAT4X4 m_xmf4x4Projection = Matrix4x4::Identity();

	D3D12_VIEWPORT m_d3dViewport{ 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT, 0.0f, 1.0f };
	D3D12_RECT m_d3dScissorRect{ 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT };

	Player* m_pPlayer = nullptr;

	ID3D12Resource* m_pd3dcbCamera = nullptr;
	VS_CB_CAMERA_INFO* m_pcbMappedCamera = nullptr;

public:

	void SetViewport(const int& xTopLeft, const int& yTopLeft, const int& nWidth, const int& nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(const LONG& xLeft, const LONG& yTop, const LONG& xRight, const LONG& yBottom);
	virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	void GenerateViewMatrix();
	void GenerateViewMatrix(const XMFLOAT3& xmf3Position, const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up);
	void RegenerateViewMatrix();
	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);

	virtual void Move(const XMFLOAT3& xmf3Shift);
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
	virtual void Update(const XMFLOAT3& xmf3LookAt, float fTimeElapsed);
	virtual void SetLookAt(const XMFLOAT3& xmf3LookAt);

	void SetPlayer(Player* pPlayer);
	Player* GetPlayer();

	void SetMode(const DWORD& nMode);
	const DWORD& GetMode();

	void SetPosition(const XMFLOAT3& xmf3Position);
	const XMFLOAT3& GetPosition();

	void SetLookAtPosition(const XMFLOAT3& xmf3LookAtWorld);
	const XMFLOAT3& GetLookAtPosition();

	void SetRightVector(const XMFLOAT3& xmf3Right);
	const XMFLOAT3& GetRightVector();

	void SetUpVector(const XMFLOAT3& xmf3Up);
	const XMFLOAT3& GetUpVector();

	void SetLookVector(const XMFLOAT3& xmf3Look);
	const XMFLOAT3& GetLookVector();

	const float& GetPitch();
	const float& GetRoll();
	const float& GetYaw();

	void SetOffset(const XMFLOAT3& xmf3Offset);
	const XMFLOAT3& GetOffset();

	void SetTimeLag(float fTimeLag);
	float GetTimeLag();

	const XMFLOAT4X4& GetViewMatrix();
	const XMFLOAT4X4& GetProjectionMatrix();
	const D3D12_VIEWPORT& GetViewport();
	const D3D12_RECT& GetScissorRect();

};

class SpaceShipCamera : public Camera
{
public:
	SpaceShipCamera(Camera* pCamera);
	virtual ~SpaceShipCamera() = default;

	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};

class FirstPersonCamera : public Camera
{
public:
	FirstPersonCamera(Camera* pCamera);
	virtual ~FirstPersonCamera() = default;

	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};

class ThirdPersonCamera : public Camera
{
public:
	ThirdPersonCamera(Camera* pCamera);
	virtual ~ThirdPersonCamera() = default;

	virtual void Update(const XMFLOAT3& xmf3LookAt, float fTimeElapsed);
	virtual void SetLookAt(const XMFLOAT3& xmf3LookAt);
};