//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------

#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "WayPoint.h"

#define DIR_FORWARD					0x01
#define DIR_BACKWARD				0x02
#define DIR_LEFT					0x04
#define DIR_RIGHT					0x08
#define DIR_UP						0x10
#define DIR_DOWN					0x20

class Shader;
class AnimationController;
class Texture;
class SkinnedMesh;
class Material;
class Mesh;
class ModelInfo;
class CubeObject;
class Object
{
private:
	int								m_nReferences = 0;

public:
	void AddRef();
	void Release();

public:
	Object();
	Object(int nMaterials);
    virtual ~Object();

public:
	char							m_pstrFrameName[64];

	Mesh							*m_pMesh = nullptr;

	int								m_nMaterials = 0;
	Material						**m_ppMaterials = nullptr;

	XMFLOAT4X4						m_xmf4x4ToParent;
	XMFLOAT4X4						m_xmf4x4World;

	Object 					*m_pParent = nullptr;
	Object 					*m_pChild = nullptr;
	Object 					*m_pSibling = nullptr;


	Object* head = nullptr;	//머리
	Object* rHand = nullptr;	//오른손
	Object* lHand = nullptr;	//왼손
	Object* lFoot = nullptr;	//왼발
	Object* rFoot = nullptr;	//오른발
	Object* spine = nullptr;	//척추 중심
	XMFLOAT3 particleDir{ 0.f, 0.f, 0.f };
	XMFLOAT3 scale{ 1.f, 1.f,1.f };

	float hp{ 100.f };

	bool boundBoxRender = true;
	map<string, CubeObject*> boundBoxs;
	map<string, Object*> bones;
	XMFLOAT3 oldSpinePosition;
	UINT nowState = STATE_IDLE;
	UINT attackType = 0;

	bool isActive = true;
	//충돌처리
	bool isAlive{ true };
	bool isHit{ false };
	bool isCollide{ false };
	bool canMove[4]{ true };
	WayPoint wayPoint{};
	

	BoundingOrientedBox* objectCollision = nullptr;

	void SetMesh(Mesh *pMesh);
	void SetShader(Shader *pShader);
	void SetShader(int nMaterial, Shader *pShader);
	void SetMaterial(int nMaterial, Material *pMaterial);

	void SetChild(Object *pChild, bool bReferenceUpdate=false);

	virtual void BuildMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) { }

	virtual void OnPrepareAnimate() { }
	virtual void Animate(float fTimeElapsed);

	virtual void OnPrepareRender() { }
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, Camera *pCamera=nullptr);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, Material *pMaterial);

	virtual void ReleaseUploadBuffers();

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	void SetPosition(float x, float y, float z);
	void SetLook(float x, float y, float z);
	void SetRight(float x, float y, float z);
	void SetUp(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);
	void SetLook(XMFLOAT3 look);
	void SetRight(XMFLOAT3 right);
	void SetUp(XMFLOAT3 up);
	void SetScale(float x, float y, float z);
	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);
	void Move(XMFLOAT3 xmfShift);
	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3 *pxmf3Axis, float fAngle);
	void Rotate(XMFLOAT4 *pxmf4Quaternion);

	Object *GetParent() { return(m_pParent); }
	void UpdateTransform(XMFLOAT4X4 *pxmf4x4Parent=nullptr);
	Object *FindFrame(char *pstrFrameName);

	Texture *FindReplicatedTexture(_TCHAR *pstrTextureName);

	UINT GetMeshType() { return((m_pMesh) ? m_pMesh->GetType() : 0x00); }

public:
	AnimationController 			*m_pSkinnedAnimationController = nullptr;

	SkinnedMesh *FindSkinnedMesh(char *pstrSkinnedMeshName);
	void FindAndSetSkinnedMesh(SkinnedMesh **ppSkinnedMeshes, int *pnSkinnedMesh);

	void SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet);
	void SetTrackAnimationPosition(int nAnimationTrack, float fPosition);

	void LoadMaterialsFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, Object *pParent, FILE *pInFile, Shader *pShader);

	static void LoadAnimationFromFile(FILE *pInFile, ModelInfo *pLoadedModel);
	static Object *LoadFrameHierarchyFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, Object *pParent, FILE *pInFile, Shader *pShader, int *pnSkinnedMeshes);

	static ModelInfo *LoadGeometryAndAnimationFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, char *pstrFileName, Shader *pShader);

	static void PrintFrameInfo(Object *pGameObject, Object *pParent);

	virtual void MoveTo(XMFLOAT3 destination);
	virtual void UpdateWayPoints();
};