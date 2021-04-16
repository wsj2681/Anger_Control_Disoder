#pragma once
#include "Camera.h"

class AnimationController;
class Mesh;
class Material;
class Texture;
class SkinnedMesh;
class Shader;
class ModelInfo;

class Object
{
public:

	Object() = default;
	Object(const Object&) = delete;
	Object& operator=(const Object&) = delete;
	virtual ~Object() = default;

private:

protected:

	char m_pstrFrameName[64];

	Mesh* m_pMesh = nullptr;

	int m_nMaterials = 0;
	Material** m_ppMaterials = nullptr;

	XMFLOAT4X4 m_xmf4x4ToParent;
	XMFLOAT4X4 m_xmf4x4World;

	Object* m_pParent = nullptr;
	Object* m_pChild = nullptr;
	Object* m_pSibling = nullptr;

public:	/* Method */

	virtual void BuildMaterials(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);

	virtual void OnPrepareAnimate();
	virtual void Animate(float fTimeElapsed);

	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera = nullptr);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, Material* pMaterial);
	virtual void ReleaseShaderVariables();

	virtual void ReleaseUploadBuffers();

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3* pxmf3Axis, float fAngle);
	void Rotate(XMFLOAT4* pxmf4Quaternion);

	void UpdateTransform(XMFLOAT4X4* pxmf4x4Parent = nullptr);
	Object* FindFrame(const char* pstrFrameName);
	Texture* FindReplicatedTexture(_TCHAR* pstrTextureName);

public: /* Get Set */

	Object* GetParent();
	const UINT& GetMeshType();

	void SetPosition(const float& x, const float& y, const float& z);
	void SetPosition(const XMFLOAT3& position);
	const XMFLOAT3& GetPosition();

	void SetLook(const float& x, const float& y, const float& z);
	void SetLook(const XMFLOAT3& look);
	const XMFLOAT3& GetLook();

	void SetUp(const float& x, const float& y, const float& z);
	void SetUp(const XMFLOAT3& up);
	const XMFLOAT3& GetUp();

	void SetRight(const float& x, const float& y, const float& z);
	void SetRight(const XMFLOAT3& right);
	const XMFLOAT3& GetRight();

	void SetScale(const float& x, const float& y, const float& z);
	void SetScale(const XMFLOAT3& scale);

public: /* Animation Method */

	AnimationController* m_pSkinnedAnimationController = nullptr;

	SkinnedMesh* FindSkinnedMesh(char* pstrSkinnedMeshName);
	void FindAndSetSkinnedMesh(SkinnedMesh** ppSkinnedMeshes, int* pnSkinnedMesh);

	void SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet);
	void SetTrackAnimationPosition(int nAnimationTrack, float fPosition);

public: /* Model Method */

	static void LoadAnimationFromFile(FILE* pInFile, ModelInfo* pLoadedModel);
	static Object* LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, Object* pParent, FILE* pInFile, Shader* pShader, int* pnSkinnedMeshes);
	static ModelInfo* LoadGeometryAndAnimationFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, char* pstrFileName, Shader* pShader);
	static void PrintFrameInfo(Object* pGameObject, Object* pParent);

public: /* Custom Method */

	virtual void MoveTo(const XMFLOAT3& destination);
	virtual void MoveTo(const float& x, const float& y, const float& z);
	virtual void UpdateWayPoints();

};

