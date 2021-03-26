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


class Camera
{
public:

	Camera();
	Camera(const Camera&) = delete;
	Camera(Camera* camera);
	Camera& operator=(const Camera&) = delete;
	virtual ~Camera();

protected:

	XMFLOAT3 position{ 0.f, 0.f, 0.f };
	XMFLOAT3 right{ 1.f, 0.f, 0.f };
	XMFLOAT3 up{ 0.f, 1.f, 0.f };
	XMFLOAT3 look{ 0.f, 0.f, 1.f };

	float pitch = 0.f;
	float roll = 0.f;
	float yaw = 0.f;

	DWORD cameraMode;

	XMFLOAT3 lookAtWorld{ 0.f, 0.f, 0.f };
	XMFLOAT3 offset{ 0.f, 0.f, 0.f };
	float timeLag = 0.f;

	XMFLOAT4X4 view;
	XMFLOAT4X4 Projection;

	D3D12_VIEWPORT viewport;
	D3D12_RECT sissorRect;

	ID3D12Resource* cbCamera = nullptr;
	VS_CB_CAMERA_INFO* mappedCamera = nullptr;

public:

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);

	void GenerateViewMatrix();
	void GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up);
	void RegenerateViewMatrix();

	virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList);

	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);


};
