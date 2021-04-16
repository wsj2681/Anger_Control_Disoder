#include "framework.h"
#include "Shader.h"
#include "AnimationSet.h"
#include "AnimationController.h"
#include "Object.h"
#include "Mesh.h"
#include "SkinnedMesh.h"
#include "ModelInfo.h"
#include "Material.h"

void Object::AddRef()
{
	m_nReferences++;

	if (m_pSibling) m_pSibling->AddRef();
	if (m_pChild) m_pChild->AddRef();
}

void Object::Release()
{
	if (m_pChild) m_pChild->Release();
	if (m_pSibling) m_pSibling->Release();

	if (--m_nReferences <= 0) delete this;
}

void Object::BuildMaterials(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void Object::OnPrepareAnimate()
{
}

void Object::Animate(float fTimeElapsed)
{
}

void Object::OnPrepareRender()
{
}

void Object::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	if (this->isActive)
	{
		if (m_pSkinnedAnimationController) m_pSkinnedAnimationController->UpdateShaderVariables(pd3dCommandList);

		if (m_pMesh)
		{
			UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);

			if (m_nMaterials > 0)
			{
				for (int i = 0; i < m_nMaterials; i++)
				{
					if (m_ppMaterials[i])
					{
						if (m_ppMaterials[i]->m_pShader) m_ppMaterials[i]->m_pShader->Render(pd3dCommandList, pCamera);
						m_ppMaterials[i]->UpdateShaderVariable(pd3dCommandList);
					}

					m_pMesh->Render(pd3dCommandList, i);
				}
			}
		}

		if (m_pSibling) m_pSibling->Render(pd3dCommandList, pCamera);
		if (m_pChild) m_pChild->Render(pd3dCommandList, pCamera);
	}
}

void Object::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void Object::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void Object::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);
}

void Object::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, Material* pMaterial)
{
}

void Object::ReleaseShaderVariables()
{
}

void Object::ReleaseUploadBuffers()
{
	if (m_pMesh) m_pMesh->ReleaseUploadBuffers();

	for (int i = 0; i < m_nMaterials; i++)
	{
		if (m_ppMaterials[i]) m_ppMaterials[i]->ReleaseUploadBuffers();
	}

	if (m_pSibling) m_pSibling->ReleaseUploadBuffers();
	if (m_pChild) m_pChild->ReleaseUploadBuffers();
}

void Object::MoveStrafe(float fDistance)
{
}

void Object::MoveUp(float fDistance)
{
}

void Object::MoveForward(float fDistance)
{
}

void Object::Rotate(float fPitch, float fYaw, float fRoll)
{
}

void Object::Rotate(XMFLOAT3* pxmf3Axis, float fAngle)
{
}

void Object::Rotate(XMFLOAT4* pxmf4Quaternion)
{
}

void Object::UpdateTransform(XMFLOAT4X4* pxmf4x4Parent)
{
}

Object* Object::FindFrame(const char* pstrFrameName)
{
	return nullptr;
}

Texture* Object::FindReplicatedTexture(_TCHAR* pstrTextureName)
{
	for (int i = 0; i < m_nMaterials; i++)
	{
		if (m_ppMaterials[i])
		{
			for (int j = 0; j < m_ppMaterials[i]->m_nTextures; j++)
			{
				if (m_ppMaterials[i]->m_ppTextures[j])
				{
					if (!_tcsncmp(m_ppMaterials[i]->m_ppstrTextureNames[j], pstrTextureName, _tcslen(pstrTextureName)))
					{
						return(m_ppMaterials[i]->m_ppTextures[j]);
					}
				}
			}
		}
	}
	Texture* pTexture = nullptr;
	if (m_pSibling) 
		if (pTexture = m_pSibling->FindReplicatedTexture(pstrTextureName)) 
			return(pTexture);
	
	if (m_pChild) 
		if (pTexture = m_pChild->FindReplicatedTexture(pstrTextureName)) 
			return(pTexture);

	return nullptr;
}

void Object::SetMesh(Mesh* pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}

void Object::SetShader(Shader* pShader)
{
	m_nMaterials = 1;
	m_ppMaterials = new Material * [m_nMaterials];
	m_ppMaterials[0] = new Material(0);
	m_ppMaterials[0]->SetShader(pShader);
}

void Object::SetShader(int nMaterial, Shader* pShader)
{
	if (m_ppMaterials[nMaterial]) m_ppMaterials[nMaterial]->SetShader(pShader);
}

void Object::SetMaterial(int nMaterial, Material* pMaterial)
{
	if (m_ppMaterials[nMaterial]) m_ppMaterials[nMaterial]->Release();
	m_ppMaterials[nMaterial] = pMaterial;
	if (m_ppMaterials[nMaterial]) m_ppMaterials[nMaterial]->AddRef();
}

void Object::SetChild(Object* pChild, bool bReferenceUpdate)
{
	if (pChild)
	{
		pChild->m_pParent = this;
		if (bReferenceUpdate) pChild->AddRef();
	}
	if (m_pChild)
	{
		if (pChild) pChild->m_pSibling = m_pChild->m_pSibling;
		m_pChild->m_pSibling = pChild;
	}
	else
	{
		m_pChild = pChild;
	}
}

void Object::SetToParent(const XMFLOAT4X4& toParent)
{
	this->m_xmf4x4ToParent = toParent;
}

const XMFLOAT4X4& Object::GetToParent()
{
	return this->m_xmf4x4ToParent;
}

void Object::SetWorld(const XMFLOAT4X4& world)
{
	this->m_xmf4x4World = world;
}

const XMFLOAT4X4& Object::GetWorld()
{
	return this->m_xmf4x4World;
}

Object* Object::GetParent()
{
	return this->m_pParent;
}

const UINT& Object::GetMeshType()
{
	
}

void Object::SetPosition(const float& x, const float& y, const float& z)
{
	m_xmf4x4ToParent._41 = x;
	m_xmf4x4ToParent._42 = y;
	m_xmf4x4ToParent._43 = z;

	UpdateTransform(nullptr);
}

void Object::SetPosition(const XMFLOAT3& position)
{
	SetPosition(position.x, position.y, position.z);
}

const XMFLOAT3& Object::GetPosition()
{
	return XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43);
}

void Object::SetLook(const float& x, const float& y, const float& z)
{
	m_xmf4x4ToParent._31 = x;
	m_xmf4x4ToParent._32 = y;
	m_xmf4x4ToParent._33 = z;

	UpdateTransform(nullptr);
}

void Object::SetLook(const XMFLOAT3& look)
{
	SetPosition(look.x, look.y, look.z);
}

const XMFLOAT3& Object::GetLook()
{
	return Vector3::Normalize(XMFLOAT3(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33));
}

void Object::SetUp(const float& x, const float& y, const float& z)
{
	m_xmf4x4ToParent._21 = x;
	m_xmf4x4ToParent._22 = y;
	m_xmf4x4ToParent._23 = z;

	UpdateTransform(nullptr);
}

void Object::SetUp(const XMFLOAT3& up)
{
	SetPosition(up.x, up.y, up.z);
}

const XMFLOAT3& Object::GetUp()
{
	return Vector3::Normalize(XMFLOAT3(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23));
}

void Object::SetRight(const float& x, const float& y, const float& z)
{
	m_xmf4x4ToParent._11 = x;
	m_xmf4x4ToParent._12 = y;
	m_xmf4x4ToParent._13 = z;

	UpdateTransform(nullptr);
}

void Object::SetRight(const XMFLOAT3& right)
{
	SetPosition(right.x, right.y, right.z);
}

const XMFLOAT3& Object::GetRight()
{
	return Vector3::Normalize(XMFLOAT3(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13));
}

void Object::SetScale(const float& x, const float& y, const float& z)
{
}

void Object::SetScale(const XMFLOAT3& scale)
{
}

SkinnedMesh* Object::FindSkinnedMesh(char* pstrSkinnedMeshName)
{
	return nullptr;
}

void Object::FindAndSetSkinnedMesh(SkinnedMesh** ppSkinnedMeshes, int* pnSkinnedMesh)
{
}

void Object::SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet)
{
}

void Object::SetTrackAnimationPosition(int nAnimationTrack, float fPosition)
{
}

void Object::LoadAnimationFromFile(FILE* pInFile, ModelInfo* pLoadedModel)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	int nAnimationSets = 0;

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<AnimationSets>:"))
		{
			nAnimationSets = ::ReadIntegerFromFile(pInFile);
			pLoadedModel->m_pAnimationSets = new AnimationSets(nAnimationSets);
		}
		else if (!strcmp(pstrToken, "<FrameNames>:"))
		{
			pLoadedModel->m_pAnimationSets->m_nAnimatedBoneFrames = ::ReadIntegerFromFile(pInFile);
			pLoadedModel->m_pAnimationSets->m_ppAnimatedBoneFrameCaches = new Object * [pLoadedModel->m_pAnimationSets->m_nAnimatedBoneFrames];

			for (int j = 0; j < pLoadedModel->m_pAnimationSets->m_nAnimatedBoneFrames; j++)
			{
				::ReadStringFromFile(pInFile, pstrToken);
				pLoadedModel->m_pAnimationSets->m_ppAnimatedBoneFrameCaches[j] = pLoadedModel->m_pModelRootObject->FindFrame(pstrToken);

#ifdef _WITH_DEBUG_SKINNING_BONE
				TCHAR pstrDebug[256] = { 0 };
				TCHAR pwstrAnimationBoneName[64] = { 0 };
				TCHAR pwstrBoneCacheName[64] = { 0 };
				size_t nConverted = 0;
				mbstowcs_s(&nConverted, pwstrAnimationBoneName, 64, pstrToken, _TRUNCATE);
				mbstowcs_s(&nConverted, pwstrBoneCacheName, 64, pLoadedModel->m_ppAnimatedBoneFrameCaches[j]->m_pstrFrameName, _TRUNCATE);
				_stprintf_s(pstrDebug, 256, _T("AnimationBoneFrame:: Cache(%s) AnimationBone(%s)\n"), pwstrBoneCacheName, pwstrAnimationBoneName);
				OutputDebugString(pstrDebug);
#endif
			}
		}
		else if (!strcmp(pstrToken, "<AnimationSet>:"))
		{
			int nAnimationSet = ::ReadIntegerFromFile(pInFile);

			::ReadStringFromFile(pInFile, pstrToken); //Animation Set Name

			float fLength = ::ReadFloatFromFile(pInFile);
			int nFramesPerSecond = ::ReadIntegerFromFile(pInFile);
			int nKeyFrames = ::ReadIntegerFromFile(pInFile);

			pLoadedModel->m_pAnimationSets->m_pAnimationSets[nAnimationSet] = new CAnimationSet(fLength, nFramesPerSecond, nKeyFrames, pLoadedModel->m_pAnimationSets->m_nAnimatedBoneFrames, pstrToken);

			for (int i = 0; i < nKeyFrames; i++)
			{
				::ReadStringFromFile(pInFile, pstrToken);
				if (!strcmp(pstrToken, "<Transforms>:"))
				{
					int nKey = ::ReadIntegerFromFile(pInFile); //i
					float fKeyTime = ::ReadFloatFromFile(pInFile);

					CAnimationSet* pAnimationSet = pLoadedModel->m_pAnimationSets->m_pAnimationSets[nAnimationSet];
					pAnimationSet->m_pfKeyFrameTimes[i] = fKeyTime;
					nReads = (UINT)::fread(pAnimationSet->m_ppxmf4x4KeyFrameTransforms[i], sizeof(XMFLOAT4X4), pLoadedModel->m_pAnimationSets->m_nAnimatedBoneFrames, pInFile);
				}
			}
		}
		else if (!strcmp(pstrToken, "</AnimationSets>"))
		{
			break;
		}
	}
}

void Object::LoadMaterialsFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, Object* pParent, FILE* pInFile, Shader* pShader)
{
	char pstrToken[64] = { '\0' };
	int nMaterial = 0;
	UINT nReads = 0;

	m_nMaterials = ReadIntegerFromFile(pInFile);

	m_ppMaterials = new Material * [m_nMaterials];
	for (int i = 0; i < m_nMaterials; i++) m_ppMaterials[i] = NULL;

	Material* pMaterial = NULL;

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);

		if (!strcmp(pstrToken, "<Material>:"))
		{
			nMaterial = ReadIntegerFromFile(pInFile);

			pMaterial = new Material(7); //0:Albedo, 1:Specular, 2:Metallic, 3:Normal, 4:Emission, 5:DetailAlbedo, 6:DetailNormal

			if (!pShader)
			{
				UINT nMeshType = GetMeshType();
				if (nMeshType & VERTEXT_NORMAL_TANGENT_TEXTURE)
				{
					if (nMeshType & VERTEXT_BONE_INDEX_WEIGHT)
					{
						pMaterial->SetSkinnedAnimationShader();
					}
					else
					{
						pMaterial->SetStandardShader();
					}
				}
			}
			SetMaterial(nMaterial, pMaterial);
		}
		else if (!strcmp(pstrToken, "<AlbedoColor>:"))
		{
			nReads = (UINT)::fread(&(pMaterial->m_xmf4AlbedoColor), sizeof(float), 4, pInFile);
		}
		else if (!strcmp(pstrToken, "<EmissiveColor>:"))
		{
			nReads = (UINT)::fread(&(pMaterial->m_xmf4EmissiveColor), sizeof(float), 4, pInFile);
		}
		else if (!strcmp(pstrToken, "<SpecularColor>:"))
		{
			nReads = (UINT)::fread(&(pMaterial->m_xmf4SpecularColor), sizeof(float), 4, pInFile);
		}
		else if (!strcmp(pstrToken, "<Glossiness>:"))
		{
			nReads = (UINT)::fread(&(pMaterial->m_fGlossiness), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Smoothness>:"))
		{
			nReads = (UINT)::fread(&(pMaterial->m_fSmoothness), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Metallic>:"))
		{
			nReads = (UINT)::fread(&(pMaterial->m_fSpecularHighlight), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<SpecularHighlight>:"))
		{
			nReads = (UINT)::fread(&(pMaterial->m_fMetallic), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<GlossyReflection>:"))
		{
			nReads = (UINT)::fread(&(pMaterial->m_fGlossyReflection), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<AlbedoMap>:"))
		{
			pMaterial->LoadTextureFromFile(pd3dDevice, pd3dCommandList, MATERIAL_ALBEDO_MAP, 3, pMaterial->m_ppstrTextureNames[0], &(pMaterial->m_ppTextures[0]), pParent, pInFile, pShader);
		}
		else if (!strcmp(pstrToken, "<SpecularMap>:"))
		{
			m_ppMaterials[nMaterial]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, MATERIAL_SPECULAR_MAP, 4, pMaterial->m_ppstrTextureNames[1], &(pMaterial->m_ppTextures[1]), pParent, pInFile, pShader);
		}
		else if (!strcmp(pstrToken, "<NormalMap>:"))
		{
			m_ppMaterials[nMaterial]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, MATERIAL_NORMAL_MAP, 5, pMaterial->m_ppstrTextureNames[2], &(pMaterial->m_ppTextures[2]), pParent, pInFile, pShader);
		}
		else if (!strcmp(pstrToken, "<MetallicMap>:"))
		{
			m_ppMaterials[nMaterial]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, MATERIAL_METALLIC_MAP, 6, pMaterial->m_ppstrTextureNames[3], &(pMaterial->m_ppTextures[3]), pParent, pInFile, pShader);
		}
		else if (!strcmp(pstrToken, "<EmissionMap>:"))
		{
			m_ppMaterials[nMaterial]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, MATERIAL_EMISSION_MAP, 7, pMaterial->m_ppstrTextureNames[4], &(pMaterial->m_ppTextures[4]), pParent, pInFile, pShader);
		}
		else if (!strcmp(pstrToken, "<DetailAlbedoMap>:"))
		{
			m_ppMaterials[nMaterial]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, MATERIAL_DETAIL_ALBEDO_MAP, 8, pMaterial->m_ppstrTextureNames[5], &(pMaterial->m_ppTextures[5]), pParent, pInFile, pShader);
		}
		else if (!strcmp(pstrToken, "<DetailNormalMap>:"))
		{
			m_ppMaterials[nMaterial]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, MATERIAL_DETAIL_NORMAL_MAP, 9, pMaterial->m_ppstrTextureNames[6], &(pMaterial->m_ppTextures[6]), pParent, pInFile, pShader);
		}
		else if (!strcmp(pstrToken, "</Materials>"))
		{
			break;
		}
	}
}

Object* Object::LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, Object* pParent, FILE* pInFile, Shader* pShader, int* pnSkinnedMeshes)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	int nFrame = 0, nTextures = 0;

	Object* pGameObject = new Object();

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<Frame>:"))
		{
			nFrame = ::ReadIntegerFromFile(pInFile);
			nTextures = ::ReadIntegerFromFile(pInFile);

			::ReadStringFromFile(pInFile, pGameObject->m_pstrFrameName);
		}
		else if (!strcmp(pstrToken, "<Transform>:"))
		{
			XMFLOAT3 xmf3Position, xmf3Rotation, xmf3Scale;
			XMFLOAT4 xmf4Rotation;
			nReads = (UINT)::fread(&xmf3Position, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf3Rotation, sizeof(float), 3, pInFile); //Euler Angle
			nReads = (UINT)::fread(&xmf3Scale, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf4Rotation, sizeof(float), 4, pInFile); //Quaternion
		}
		else if (!strcmp(pstrToken, "<TransformMatrix>:"))
		{
			nReads = (UINT)::fread(&pGameObject->m_xmf4x4ToParent, sizeof(float), 16, pInFile);
		}
		else if (!strcmp(pstrToken, "<Mesh>:"))
		{
			StandardMesh* pMesh = new StandardMesh(pd3dDevice, pd3dCommandList);
			pMesh->LoadMeshFromFile(pd3dDevice, pd3dCommandList, pInFile);
			pGameObject->SetMesh(pMesh);
		}
		else if (!strcmp(pstrToken, "<SkinningInfo>:"))
		{
			if (pnSkinnedMeshes) (*pnSkinnedMeshes)++;

			SkinnedMesh* pSkinnedMesh = new SkinnedMesh(pd3dDevice, pd3dCommandList);
			pSkinnedMesh->LoadSkinInfoFromFile(pd3dDevice, pd3dCommandList, pInFile);
			pSkinnedMesh->CreateShaderVariables(pd3dDevice, pd3dCommandList);

			::ReadStringFromFile(pInFile, pstrToken); //<Mesh>:
			if (!strcmp(pstrToken, "<Mesh>:")) pSkinnedMesh->LoadMeshFromFile(pd3dDevice, pd3dCommandList, pInFile);

			pGameObject->SetMesh(pSkinnedMesh);
		}
		else if (!strcmp(pstrToken, "<Materials>:"))
		{
			pGameObject->LoadMaterialsFromFile(pd3dDevice, pd3dCommandList, pParent, pInFile, pShader);
		}
		else if (!strcmp(pstrToken, "<Children>:"))
		{
			int nChilds = ::ReadIntegerFromFile(pInFile);
			if (nChilds > 0)
			{
				for (int i = 0; i < nChilds; i++)
				{
					Object* pChild = Object::LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pGameObject, pInFile, pShader, pnSkinnedMeshes);
					if (pChild) pGameObject->SetChild(pChild);
#ifdef _WITH_DEBUG_FRAME_HIERARCHY
					TCHAR pstrDebug[256] = { 0 };
					_stprintf_s(pstrDebug, 256, "(Frame: %p) (Parent: %p)\n"), pChild, pGameObject);
					OutputDebugString(pstrDebug);
#endif
				}
			}
		}
		else if (!strcmp(pstrToken, "</Frame>"))
		{
			break;
		}
	}
	return(pGameObject);
}

ModelInfo* Object::LoadGeometryAndAnimationFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, char* pstrFileName, Shader* pShader)
{
	FILE* pInFile = NULL;
	::fopen_s(&pInFile, pstrFileName, "rb");
	::rewind(pInFile);

	ModelInfo* pLoadedModel = new ModelInfo();

	char pstrToken[64] = { '\0' };

	for (; ; )
	{
		if (::ReadStringFromFile(pInFile, pstrToken))
		{
			if (!strcmp(pstrToken, "<Hierarchy>:"))
			{
				pLoadedModel->m_pModelRootObject = Object::LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, NULL, pInFile, pShader, &pLoadedModel->m_nSkinnedMeshes);
				::ReadStringFromFile(pInFile, pstrToken); //"</Hierarchy>"
			}
			else if (!strcmp(pstrToken, "<Animation>:"))
			{
				Object::LoadAnimationFromFile(pInFile, pLoadedModel);
				pLoadedModel->PrepareSkinning();
			}
			else if (!strcmp(pstrToken, "</Animation>:"))
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

#ifdef _WITH_DEBUG_FRAME_HIERARCHY
	TCHAR pstrDebug[256] = { 0 };
	_stprintf_s(pstrDebug, 256, "Frame Hierarchy\n"));
	OutputDebugString(pstrDebug);

	CGameObject::PrintFrameInfo(pGameObject, NULL);
#endif

	return(pLoadedModel);
}

void Object::PrintFrameInfo(Object* pGameObject, Object* pParent)
{
	TCHAR pstrDebug[256] = { 0 };

	_stprintf_s(pstrDebug, 256, _T("(Frame: %p) (Parent: %p)\n"), pGameObject, pParent);
	OutputDebugString(pstrDebug);

	if (pGameObject->m_pSibling) Object::PrintFrameInfo(pGameObject->m_pSibling, pParent);
	if (pGameObject->m_pChild) Object::PrintFrameInfo(pGameObject->m_pChild, pGameObject);
}

void Object::MoveTo(const XMFLOAT3& destination)
{
	wayPoint.SetNowState(STATE_MOVE);
	UINT animation = wayPoint.GetAnimations();
	m_pSkinnedAnimationController->SetTrackAnimationSet(0, animation);
	XMFLOAT3 comparePosition = GetPosition();
	comparePosition = Vector3::Subtract(destination, comparePosition);

	// 이동
	XMFLOAT3 look = GetLook();
	XMFLOAT3 position = GetPosition();
	XMFLOAT3 velocity{};

	if (wayPoint.GetNWayPoints() > 0)
	{

		XMFLOAT3 dir = Vector3::Subtract(wayPoint.GetWayPoint(wayPoint.GetCurWayPoints()), position);
		XMFLOAT3 posVelocity = velocity;
		velocity = Vector3::Normalize(dir);

		//Rotate(velocity.x, velocity.z, velocity.y); // 각 계산해서 회전

		position = GetPosition();
		position.x += velocity.x * 50 * 0.016f;	// 50 : 초당 이동 거리, 0.016 : fElapsedTime
		position.y += velocity.y * 50 * 0.016f;
		position.z += velocity.z * 50 * 0.016f;
		SetPosition(position);
	}

	if (Vector3::Length(Vector3::Subtract(wayPoint.GetWayPoint(wayPoint.GetCurWayPoints()), GetPosition())) < 1.f)
	{
		wayPoint.SetCurWayPoints(wayPoint.GetCurWayPoints() + 1);
	}
	wayPoint.SetNowState(STATE_MOVE);
}

void Object::MoveTo(const float& x, const float& y, const float& z)
{
	MoveTo(XMFLOAT3(x, y, z));
}

void Object::UpdateWayPoints()
{
	cout << "Object::UpdateWayPoints" << endl;
}
