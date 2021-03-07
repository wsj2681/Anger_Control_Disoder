#pragma once

struct VS_CB_CAMERA_INFO
{
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};

class Camera
{
public:
	Camera() = default;
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;
	virtual ~Camera() = default;

protected:

	XMFLOAT3 position{ 0.f, 0.f, 0.f };
	XMFLOAT3 right{ 1.f, 0.f, 0.f };
	XMFLOAT3 look{ 0.f, 0.f, 1.f };
	XMFLOAT3 up{ 0.f, 1.f, 0.f };

	float pitch = 0.f;
	float roll = 0.f;
	float yaw = 0.f;

	XMFLOAT3 lookAtWorld{ 0.f, 0.f, 0.f };
	XMFLOAT3 offset{ 0.f, 0.f, 0.f };

	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;

	D3D12_VIEWPORT viewport;
	D3D12_RECT sissorRect;

	ID3D12Resource* cbCamera = nullptr;
	VS_CB_CAMERA_INFO* cbMappedCamera = nullptr;

public:

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList);



};

