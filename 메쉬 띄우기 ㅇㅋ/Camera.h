#pragma once
#define ASPECT_RATIO (float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT))

//Camera Mode
#define FIRST_PERSON_CAMERA 0x01
#define SPACESHIP_CAMERA 0x02
#define THIRD_PERSON_CAMERA 0x03

#include "framework.h"

class CPlayer;

struct VS_CB_CAMERA_INFO {
	XMFLOAT4X4 m_xmf4x4View{ Matrix4x4::Identity() };
	XMFLOAT4X4 m_xmf4x4Projection{ Matrix4x4::Identity() };
};

class CCamera {
protected:
	XMFLOAT3 m_xmf3Position{ XMFLOAT3(0.f, 0.f, 0.f) };
	XMFLOAT3 m_xmf3Right{ XMFLOAT3(1.f, 0.f, 0.f) };
	XMFLOAT3 m_xmf3Up{ XMFLOAT3(0.f, 1.f, 0.f) };
	XMFLOAT3 m_xmf3Look{ XMFLOAT3(0.f, 0.f, 1.f) };

	float m_fPitch{ 0.f };
	float m_fRoll{ 0.f };
	float m_fYaw{ 0.f };

	DWORD m_nMode{ 0x00 };

	XMFLOAT3 m_xmf3LookAtWorld{ XMFLOAT3(0.f, 0.f, 0.f) };
	XMFLOAT3 m_xmf3Offset{ XMFLOAT3(0.f, 0.f, 0.f) };


	float m_fTimeLag{ 0.f };

	//카메라 변환 행렬
	XMFLOAT4X4 m_xmf4x4View{ Matrix4x4::Identity() };
	XMFLOAT4X4 m_xmf4x4Projection{ Matrix4x4::Identity() };

	//뷰포트와 씨저 사각형
	D3D12_VIEWPORT m_d3dViewport{ 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT, 0.0f, 1.0f };
	D3D12_RECT m_d3dScissorRect{ 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT };

	CPlayer* m_pPlayer{ nullptr };

public:
	CCamera();
	CCamera(CCamera* pCamera);
	virtual ~CCamera();

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);
	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);

	void GenerateViewMatrix();
	void GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up);

	void RegenerateViewMatrix();

	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	CPlayer* GetPlayer() { return(m_pPlayer); }

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
	XMFLOAT3& GetOffset() { return(m_xmf3Offset); }

	void SetTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; }
	float GetTimeLag() { return(m_fTimeLag); }XMFLOAT4X4 GetViewMatrix() { return(m_xmf4x4View); }

	XMFLOAT4X4 GetProjectionMatrix() { return(m_xmf4x4Projection); }
	D3D12_VIEWPORT GetViewport() { return(m_d3dViewport); }
	D3D12_RECT GetScissorRect() { return(m_d3dScissorRect); }
	//카메라를 xmf3Shift 만큼 이동한다.

	virtual void Move(const XMFLOAT3& xmf3Shift)
	{
		m_xmf3Position.x += xmf3Shift.x;
		m_xmf3Position.y += xmf3Shift.y;
		m_xmf3Position.z += xmf3Shift.z;
	}

	//카메라를 x-축, y-축, z-축으로 회전하는 가상함수이다.
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) { }
	//카메라의 이동, 회전에 따라 카메라의 정보를 갱신하는 가상함수이다
	virtual void Update(XMFLOAT3& xmf3LookAt, float fTimeElapsed) { }
	//3인칭 카메라에서 카메라가 바라보는 지점을 설정한다. 일반적으로 플레이어를 바라보도록 설정한다.
	virtual void SetLookAt(XMFLOAT3& xmf3LookAt) { }




	virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList);


};

class CSpaceShipCamera : public CCamera {
public:
	CSpaceShipCamera(CCamera* pCamera);
	virtual ~CSpaceShipCamera() { }
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};

class CFirstPersonCamera : public CCamera {
public:
	CFirstPersonCamera(CCamera* pCamera);
	virtual ~CFirstPersonCamera() { }
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};

class CThirdPersonCamera : public CCamera {
public:
	CThirdPersonCamera(CCamera* pCamera);
	virtual ~CThirdPersonCamera() { }
	virtual void Update(XMFLOAT3& xmf3LookAt, float fTimeElapsed);
	virtual void SetLookAt(XMFLOAT3& vLookAt);
};