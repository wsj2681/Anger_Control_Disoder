#pragma once

//ī�޶� ��� ���۸� ���� ����ü
//struct VS_CB_CAMERA_INFO
//{
//	XMFLOAT4X4 xm4View;
//	XMFLOAT4X4 xm4Projection;
//	XMFLOAT3 xm3Position;
//};

class Camera
{
public:
	Camera();
	Camera(const Camera*);
	~Camera() = default;

protected:
	//ī���� ������ǥ��
	XMFLOAT3 xm3Position;

	// ī�޶��� ���� ��ǥ��
	XMFLOAT3	xm3Right{ 0.f, 0.f, 0.f };
	XMFLOAT3	xm3Up{ 0.f, 0.f, 0.f };
	XMFLOAT3	xm3Look{ 0.f, 0.f, 0.f };

	//ī�޶� ȸ�� ����
	float       Pitch;
	float      	Roll;
	float      	Yaw;

	//�ٶ󺸰Ե�  ��ġ ����
	XMFLOAT3	xm3LookatWorld{ 0.f, 0.f, 0.f };
	XMFLOAT3	xm3Offset{ 0.f, 0.f, 0.f };

	//ī�޶� ��ȯ ���(4x4)
	XMFLOAT4X4 xm4View = Matrix4x4::Identity();
	//���� ��ȯ ���(4x4)
	XMFLOAT4X4 xm4Projection;

	//����Ʈ�� ���� �簢��
	D3D12_VIEWPORT Viewport;
	D3D12_RECT ScissorRect;

	ID3D12Resource* RCamera;

	//VS_CB_CAMERA_INFO* MappedCamera

public:

	//ī�޶� ��ȯ ����� �����Ѵ�. 
	void GenerateViewMatrix();
	void GenerateViewMatrix(XMFLOAT3, XMFLOAT3, XMFLOAT3);

	// ī�޶��� ���� x-��(Right), y-��(Up), z-��(Look)�� ���� �����ϵ��� ������ش�.
	void RegenerateViewMatrix();


	//���� ��ȯ ����� �����Ѵ�. 
	void GenerateProjectionMatrix(float, float, float, float);

	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ, float fMaxZ);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);

	//virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList);

	void SetPosition(XMFLOAT3);
	void SetLookAtPosition(XMFLOAT3);



	const XMFLOAT3& GetPosition();
	const XMFLOAT3& GetLookAtPosition();

	const XMFLOAT3& GetRightVector();
	const XMFLOAT3& GetUpVector();
	const XMFLOAT3& GetLookVector();

	const float& GetPitch();
	const float& GetRoll();
	const float& GetYaw();


	void SetOffset(XMFLOAT3);

	const XMFLOAT3& GetOffset();

	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
	D3D12_VIEWPORT GetViewport();
	D3D12_RECT GetScissorRect();

	//ī�޶� �̵�
	void Move(const XMFLOAT3&);
	// ī�޶� �� ������ ȸ���ϴ� �Լ�
	void Rotate(float, float, float);

	//���� �Լ� 
	void Update(XMFLOAT3&, float);

	//3��Ī�� ���
	//virtual void SetLookAt(XMFLOAT3& xmf3LookAt) { }

	void CreateShaderVariables(ID3D12Device* Device, ID3D12GraphicsCommandList* CommandList);
	void ReleaseShaderVariables();
	void UpdateShaderVariables(ID3D12GraphicsCommandList* CommandList);

};