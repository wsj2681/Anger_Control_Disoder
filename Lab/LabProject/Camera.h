#pragma once

#define ASPECT_RATIO				(float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT))

constexpr UINT FIRST_PERSON_CAMERA = 1;
constexpr UINT SPACESHIP_CAMERA = 2;
constexpr UINT THIRD_PERSON_CAMERA = 3;
constexpr UINT SPACESHIP_CAMERA2 = 4;
constexpr UINT THIRD_PERSON_CAMERA2 = 5;

struct VS_CB_CAMERA_INFO
{
	XMFLOAT4X4						m_xmf4x4View;
	XMFLOAT4X4						m_xmf4x4Projection;
	XMFLOAT3						m_xmf3Position;
};

class Player;

class Camera
{
protected:
	XMFLOAT3						m_xmf3Position;
	XMFLOAT3						m_xmf3Right;
	XMFLOAT3						m_xmf3Up;
	XMFLOAT3						m_xmf3Look;

	XMFLOAT3						m_xmf3CapturedPosition;
	XMFLOAT3						m_xmf3CapturedRight;
	XMFLOAT3						m_xmf3CapturedUp;
	XMFLOAT3						m_xmf3CapturedLook;

	float           				m_fPitch;
	float           				m_fRoll;
	float           				m_fYaw;

	DWORD							m_nMode;

	XMFLOAT3						m_xmf3LookAtWorld;
	XMFLOAT3						m_xmf3Offset;
	float           				m_fTimeLag;

	XMFLOAT4X4						m_xmf4x4View;
	XMFLOAT4X4						m_xmf4x4Projection;

	D3D12_VIEWPORT					m_d3dViewport;
	D3D12_RECT						m_d3dScissorRect;

	Player							*m_pPlayer = nullptr;

	ID3D12Resource					*m_pd3dcbCamera = nullptr;
	VS_CB_CAMERA_INFO				*m_pcbMappedCamera = nullptr;

	bool isFixed = false;
	bool withPlayer = false;

public:
	Camera();
	Camera(Camera *pCamera);
	virtual ~Camera();

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);

	void GenerateViewMatrix();
	void GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up);
	void RegenerateViewMatrix();

	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);

	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);

	virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList *pd3dCommandList);

	void SetPlayer(Player *pPlayer) { m_pPlayer = pPlayer; }
	Player *GetPlayer() { return(m_pPlayer); }

	void SetMode(DWORD nMode) { m_nMode = nMode; }
	DWORD GetMode() { return(m_nMode); }

	void SetPosition(XMFLOAT3 xmf3Position) { m_xmf3Position = xmf3Position; }
	XMFLOAT3& GetPosition() { return(m_xmf3Position); }

	void SetLookAtPosition(XMFLOAT3 xmf3LookAtWorld) { m_xmf3LookAtWorld = xmf3LookAtWorld; }
	XMFLOAT3& GetLookAtPosition() { return(m_xmf3LookAtWorld); }

	XMFLOAT3& GetRightVector() { return(m_xmf3Right); }
	XMFLOAT3& GetUpVector() { return(m_xmf3Up); }
	XMFLOAT3& GetLookVector() { return(m_xmf3Look); }

	float& GetPitch() { return(m_fPitch); }
	float& GetRoll() { return(m_fRoll); }
	float& GetYaw() { return(m_fYaw); }

	void SetOffset(XMFLOAT3 xmf3Offset) { m_xmf3Offset = xmf3Offset; }
//	void SetOffset(XMFLOAT3 xmf3Offset) { m_xmf3Offset = xmf3Offset; m_xmf3Position.x += xmf3Offset.x; m_xmf3Position.y += xmf3Offset.y; m_xmf3Position.z += xmf3Offset.z; }
	XMFLOAT3& GetOffset() { return(m_xmf3Offset); }

	void SetTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; }
	float GetTimeLag() { return(m_fTimeLag); }

	XMFLOAT4X4 GetViewMatrix() { return(m_xmf4x4View); }
	XMFLOAT4X4 GetProjectionMatrix() { return(m_xmf4x4Projection); }
	D3D12_VIEWPORT GetViewport() { return(m_d3dViewport); }
	D3D12_RECT GetScissorRect() { return(m_d3dScissorRect); }

	virtual void Move(const XMFLOAT3& xmf3Shift) { m_xmf3Position.x += xmf3Shift.x; m_xmf3Position.y += xmf3Shift.y; m_xmf3Position.z += xmf3Shift.z; }
	void MoveFromShift(DWORD nDirection, float fDistance, bool bVelocity);
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) { }
	virtual void Update(XMFLOAT3& xmf3LookAt, float fTimeElapsed) { }
	virtual void SetLookAt(XMFLOAT3& xmf3LookAt) { }

	void SetFixed(bool fixed) { this->isFixed = fixed; }
	bool IsFixed() { return this->isFixed; }
	void CaptureWorld();
	void SetCaptureWorld();

public:

	XMFLOAT3 headLook;
};

class CSpaceShipCamera : public Camera
{
public:
	CSpaceShipCamera(Camera *pCamera);
	virtual ~CSpaceShipCamera() { }

	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};

class CFirstPersonCamera : public Camera
{
public:
	CFirstPersonCamera(Camera *pCamera);
	virtual ~CFirstPersonCamera() { }

	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};

class CThirdPersonCamera : public Camera
{
public:
	CThirdPersonCamera(Camera *pCamera);
	virtual ~CThirdPersonCamera() { }

	virtual void Update(XMFLOAT3& xmf3LookAt, float fTimeElapsed);
	virtual void SetLookAt(XMFLOAT3& vLookAt);
};

class CSpaceShipCamera2 : public CSpaceShipCamera
{
public:
	CSpaceShipCamera2(Camera* pCamera);
	virtual ~CSpaceShipCamera2() {}
};