#pragma once

#define ASPECT_RATIO				(float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT))

#define FIRST_PERSON_CAMERA			0x01
#define SPACESHIP_CAMERA			0x02
#define THIRD_PERSON_CAMERA			0x03

struct VS_CB_CAMERA_INFO
{
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT3 position;
};

class Player;

class Camera
{
public:

	Camera();
	Camera(const Camera&) = delete;
	Camera(Camera* camera);
	virtual ~Camera();

protected:

	XMFLOAT3 position{ 0.f, 0.f, 0.f };
	XMFLOAT3 right{ 1.f, 0.f, 0.f };
	XMFLOAT3 up{ 0.f, 1.f, 0.f };
	XMFLOAT3 look{ 0.f, 0.f, 1.f };

	float pitch = 0.f;
	float roll = 0.f;
	float yaw = 0.f;

	DWORD cameraMode = 0x00;

	XMFLOAT3 lookAtWorld{ 0.f, 0.f, 0.f };
	XMFLOAT3 offset{ 0.f, 0.f, 0.f };
	float timeLag = 0.f;

	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;

	D3D12_VIEWPORT viewport{ 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT, 0.0f, 1.0f };
	D3D12_RECT scissorRect{ 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT };

	ID3D12Resource* cbCamera = nullptr;
	VS_CB_CAMERA_INFO* mappedCamera = nullptr;

	Player* player = nullptr;

public:

	virtual void CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* commandList);

	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);

	void GenerateViewMatrix();
	void GenerateViewMatrix(const XMFLOAT3& position, const XMFLOAT3& lookAt, const XMFLOAT3& up);
	void RegenerateViewMatrix();

	virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList);

	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);

	Player* GetPlayer();
	void SetPlayer(Player* player);

	DWORD GetMode();
	void SetMode(DWORD mode);

	const XMFLOAT3& GetPosition();
	void SetPosition(const XMFLOAT3& position);
	void SetPosition(float x, float y, float z);

	const XMFLOAT3& GetRightVector();
	void SetRightVector(const XMFLOAT3& right);
	void SetRightVector(float x, float y, float z);

	const XMFLOAT3& GetLookVector();
	void SetLookVector(const XMFLOAT3& look);
	void SetLookVector(float x, float y, float z);

	const XMFLOAT3& GetUpVector();
	void GetUpVector(const XMFLOAT3& up);
	void GetUpVector(float x, float y, float z);

	const float& GetPitch();
	void SetPitch(float pitch);

	const float& GetRoll();
	void SetRoll(float roll);

	const float& GetYaw();
	void SetYaw(float yaw);

	const XMFLOAT3& GetOffset();
	void SetOffset(const XMFLOAT3& offset);
	void SetOffset(float x, float y, float z);

	const float& GetTimeLag();
	void SetTimeLag(float timeLag);

	const XMFLOAT4X4& GetViewMatrix();
	const XMFLOAT4X4& GetProjectionMatrix();
	const D3D12_VIEWPORT& GetViewport();
	const D3D12_RECT& GetScissorRect();

	virtual void Move(const XMFLOAT3& xmf3Shift);
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
	virtual void Update(const XMFLOAT3& xmf3LookAt, float fTimeElapsed);
	virtual void SetLookAt(const XMFLOAT3& xmf3LookAt);

};
