#pragma once
#include "Mesh.h"
#include "Camera.h"

class CShader;

class CGameObject{
	int m_nReferences{ 0 };

protected:
	XMFLOAT4X4 m_xmf4x4World{Matrix4x4::Identity()};

	CMesh* m_pMesh		{ nullptr };
	CShader* m_pShader	{ nullptr };

public:
	CGameObject();
	virtual ~CGameObject();

	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }
	
	void ReleaseUploadBuffers();
	
	void Rotate(XMFLOAT3* pxmf3Axis, float fAngle);

	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList) {
		XMFLOAT4X4 xmf4x4World;
		XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4World)));

		//객체의 월드 변환 행렬을 루트 상수(32-비트 값)를 통하여 셰이더 변수(상수 버퍼)로 복사한다.
		pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);
	}

	virtual void SetMesh(CMesh* pMesh);
	virtual void SetShader(CShader* pShader);
	virtual void Animate(float fTimeElapsed);
	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
};

class CRotatingObject : public CGameObject{
	XMFLOAT3 m_xmf3RotationAxis{ XMFLOAT3(0.f, 1.f, 0.f) };
	float m_fRotationSpeed{ 90.f };

public:
	CRotatingObject();
	virtual ~CRotatingObject();

	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	void SetRotationAxis(const XMFLOAT3& xmf3RotationAxis) {
		m_xmf3RotationAxis = xmf3RotationAxis;
	}
	virtual void Animate(float fTimeElapsed);
};

class MapObject : public CGameObject
{
public:
	MapObject() = default;
	~MapObject() = default;
};