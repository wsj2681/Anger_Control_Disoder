#pragma once

//ī�޶� ��� ���۸� ���� ����ü
struct VS_CB_CAMERA_INFO
{
	XMFLOAT4X4 m_xmf4x4View;
	XMFLOAT4X4 m_xmf4x4Projection;
	XMFLOAT3 m_xmf3Position;
};

class Camera
{
protected:
	//ī���� ������ǥ��
	XMFLOAT3	m_xmf3Position;

	// ī�޶��� ���� ��ǥ��
	XMFLOAT3	m_xmf3Right;
	XMFLOAT3	m_xmf3Up;
	XMFLOAT3	m_xmf3Look;

	//ī�޶� ȸ�� ����
	float       m_fPitch;
	float      	m_fRoll;
	float      	m_fYaw;

	//�ٶ󺸰Ե�  ��ġ ����
	XMFLOAT3	m_xmf3LookAtWorld;
	XMFLOAT3	m_xmf3Offset;

	//ī�޶� ��ȯ ���
	XMFLOAT4X4 m_xmf4x4View;
	//���� ��ȯ ���
	XMFLOAT4X4 m_xmf4x4Projection;

	//����Ʈ�� ���� �簢��
	D3D12_VIEWPORT m_d3dViewport;
	D3D12_RECT m_d3dScissorRect;

	ID3D12Resource* m_pd3dcbCamera = NULL;
	VS_CB_CAMERA_INFO* m_pcbMappedCamera = NULL;

public:
	Camera();
	Camera(Camera* pCamera);
	virtual ~Camera();

	//ī�޶� ��ȯ ����� �����Ѵ�. 
	void GenerateViewMatrix();
	void GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up);

	// ī�޶��� ���� x-��(Right), y-��(Up), z-��(Look)�� ���� �����ϵ��� ������ش�.
	void RegenerateViewMatrix();


	//���� ��ȯ ����� �����Ѵ�. 
	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);

	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);

	//virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList);

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


	void SetOffset(XMFLOAT3 xmf3Offset) {
		m_xmf3Offset = xmf3Offset;
		m_xmf3Position.x += xmf3Offset.x;
		m_xmf3Position.y += xmf3Offset.y;
		m_xmf3Position.z += xmf3Offset.z;
	}

	XMFLOAT3& GetOffset() { return(m_xmf3Offset); }

	XMFLOAT4X4 GetViewMatrix() { return(m_xmf4x4View); }
	XMFLOAT4X4 GetProjectionMatrix() { return(m_xmf4x4Projection); }
	D3D12_VIEWPORT GetViewport() { return(m_d3dViewport); }
	D3D12_RECT GetScissorRect() { return(m_d3dScissorRect); }

	//ī�޶� xmf3Shift��ŭ �̵�
	virtual void Move(const XMFLOAT3& xmf3Shift) { m_xmf3Position.x += xmf3Shift.x; m_xmf3Position.y += xmf3Shift.y; m_xmf3Position.z += xmf3Shift.z; }
	// ī�޶� �� ������ ȸ���ϴ� �Լ�
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) { }

	//���� �Լ� -> �ʿ��Ѱ�???
	virtual void Update(XMFLOAT3& xmf3LookAt, float fTimeElapsed) { }

	//3��Ī�� ���
	//virtual void SetLookAt(XMFLOAT3& xmf3LookAt) { }

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

};


class SpaceShipCamera : public Camera
{
public:
	SpaceShipCamera(Camera* pCamera);
	virtual ~SpaceShipCamera() { }
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};