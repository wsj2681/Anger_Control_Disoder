#pragma once
#define ASPECT_RATIO (float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT))

struct VS_CB_CAMERA_INFO
{
	XMFLOAT4X4 m_xmf4x4View = Matrix4x4::Identity();
	XMFLOAT4X4 m_xmf4x4Projection = Matrix4x4::Identity();
	XMFLOAT3 m_xmf3Position{ 0.f, 0.f, 0.f };
};

class Camera
{
public:

	Camera() = default;
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;
	virtual ~Camera() = default;

protected:

	XMFLOAT3 m_xmf3Position{ 0.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Right{ 1.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Up{ 0.f, 1.f, 0.f };
	XMFLOAT3 m_xmf3Look{ 0.f, 0.f, 1.f };

	float m_fPitch = 0.f;
	float m_fRoll = 0.f;
	float m_fYaw = 0.f;

	XMFLOAT3 m_xmf3LookAtWorld{ 0.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Offset{ 0.f, 0.f, 0.f };
	float m_fTimeLag = 0.f;

	XMFLOAT4X4 m_xmf4x4View = Matrix4x4::Identity();
	XMFLOAT4X4 m_xmf4x4Projection = Matrix4x4::Identity();

	D3D12_VIEWPORT m_d3dViewport{ 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT, 0.0f, 1.0f };
	D3D12_RECT m_d3dScissorRect{ 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT };

	ID3D12Resource* m_pd3dcbCamera = nullptr;
	VS_CB_CAMERA_INFO* m_pcbMappedCamera = nullptr;

public:

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	void GenerateViewMatrix();
	void GenerateViewMatrix(const XMFLOAT3& xmf3Position, const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up);
	void RegenerateViewMatrix();

	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);

	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);

	virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList);

	void SetPosition(const XMFLOAT3& xmf3Position);
	XMFLOAT3& GetPosition();

	void SetLookAtPosition(const XMFLOAT3& xmf3LookAtWorld);
	XMFLOAT3& GetLookAtPosition();

	XMFLOAT3& GetRightVector();
	XMFLOAT3& GetUpVector();
	XMFLOAT3& GetLookVector();

	float& GetPitch();
	float& GetRoll();
	float& GetYaw();

	void SetOffset(const XMFLOAT3& xmf3Offset);
	XMFLOAT3& GetOffset();

	void SetTimeLag(float fTimeLag);
	float GetTimeLag();

	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
	D3D12_VIEWPORT GetViewport();
	D3D12_RECT GetScissorRect();

	virtual void Move(const XMFLOAT3& xmf3Shift);
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);

	virtual void Update(const XMFLOAT3& xmf3LookAt, float fTimeElapsed);
	virtual void SetLookAt(const XMFLOAT3& xmf3LookAt);

};

