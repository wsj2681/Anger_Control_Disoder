//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------

#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "FbxSceneContext.h"

#define DIR_FORWARD					0x01
#define DIR_BACKWARD				0x02
#define DIR_LEFT					0x04
#define DIR_RIGHT					0x08
#define DIR_UP						0x10
#define DIR_DOWN					0x20

class CShader;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CAnimationController 
{
public:
	CAnimationController(FbxScene *pfbxScene);
	~CAnimationController();

public:
    float 							m_fTime = 0.0f;

	int 							m_nAnimationStacks = 0;
	FbxAnimStack 					**m_ppfbxAnimationStacks = NULL;

	int 							m_nAnimationStack = 0;

	FbxTime							*m_pfbxStartTimes = NULL;
	FbxTime							*m_pfbxStopTimes = NULL;

	FbxTime							*m_pfbxCurrentTimes = NULL;

public:
	void SetAnimationStack(FbxScene *pfbxScene, int nAnimationStack);

	void AdvanceTime(float fElapsedTime);
	FbxTime GetCurrentTime() { return(m_pfbxCurrentTimes[m_nAnimationStack]); }

	void SetPosition(int nAnimationStack, float fPosition);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CGameObject
{
private:
	int								m_nReferences = 0;

public:
	void AddRef();
	void Release();

public:
	CGameObject();
    virtual ~CGameObject();

public:
	char							m_pstrFrameName[64];

	FbxScene 						*m_pfbxScene = NULL;

	XMFLOAT4X4  					m_xmf4x4World;

	CAnimationController 			*m_pAnimationController = NULL;

	virtual void Animate(float fTimeElapsed);
	virtual void OnPrepareRender() { }
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	static void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World);
	static void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, FbxAMatrix *pfbxf4x4World);

	virtual void ReleaseUploadBuffers();

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);
	void SetScale(float x, float y, float z);

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3 *pxmf3Axis, float fAngle);
	void Rotate(XMFLOAT4 *pxmf4Quaternion);

public:
	void SetAnimationStack(int nAnimationStack) { m_pAnimationController->SetAnimationStack(m_pfbxScene, nAnimationStack); }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CAngrybotObject : public CGameObject
{
public:
	CAngrybotObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, FbxManager *pfbxSdkManager, FbxScene *pfbxScene);
	virtual ~CAngrybotObject();
};

