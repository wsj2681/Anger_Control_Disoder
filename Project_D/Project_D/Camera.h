#pragma once

//ī�޶� ��� ���۸� ���� ����ü
struct VS_CB_CAMERA_INFO
{
	XMFLOAT4X4 xm4View;
	XMFLOAT4X4 xm4Projection;
	XMFLOAT3 xm3Position;
};

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
	XMFLOAT3	xm3Right;
	XMFLOAT3	xm3Up;
	XMFLOAT3	xm3Look;

	//ī�޶� ȸ�� ����
	float       Pitch;
	float      	Roll;
	float      	Yaw;

	//�ٶ󺸰Ե�  ��ġ ����
	XMFLOAT3	xm3LookatWorld;
	XMFLOAT3	xm3Offset;

	//ī�޶� ��ȯ ���(4x4)
	XMFLOAT4X4 xm4View;
	//���� ��ȯ ���(4x4)
	XMFLOAT4X4 xm4Projection;

	//����Ʈ�� ���� �簢��
	D3D12_VIEWPORT Viewport;
	D3D12_RECT ScissorRect;

	ID3D12Resource* RCamera;
	VS_CB_CAMERA_INFO* MappedCamera;

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



	XMFLOAT3& GetPosition();
	XMFLOAT3& GetLookAtPosition();

	XMFLOAT3& GetRightVector();
	XMFLOAT3& GetUpVector();
	XMFLOAT3& GetLookVector();

	float& GetPitch();
	float& GetRoll();
	float& GetYaw();


	void SetOffset(XMFLOAT3);

	XMFLOAT3& GetOffset();

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

	virtual void CreateShaderVariables(ID3D12Device* Device, ID3D12GraphicsCommandList* CommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* CommandList);

};