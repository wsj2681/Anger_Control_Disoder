#include "framework.h"
#include "Object.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "AnimationController.h"
#include "ModelInfo.h"
#include "SkinnedMesh.h"

Object::Object()
{
	memset(frameName, 0, sizeof(frameName));

	toParent = Matrix4x4::Identity();
	world = Matrix4x4::Identity();
}

Object::Object(int materials) : Object()
{
	materialCount = materials;
	if (materialCount > 0)
	{
		this->materials = new Material * [materialCount];
		for (int i = 0; i < materialCount; ++i)
		{
			this->materials[i] = nullptr;
		}
	}
}

void Object::AddRef()
{
	this->references++;

	if (sibling)
	{
		sibling->AddRef();
	}

	if (child)
	{
		child->AddRef();
	}
}

void Object::Release()
{
	SAFE_RELEASE(child);
	SAFE_RELEASE(sibling);

	if (--references <= 0)
	{
		delete this;
	}
}

void Object::OnPrepareAnimate()
{
}

void Object::Animate(float eTime)
{
	OnPrepareAnimate();

	if (sibling)
	{
		sibling->Animate(eTime);
	}

	if (child)
	{
		child->Animate(eTime);
	}
}

void Object::OnPrepareRender()
{
}

void Object::Render(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	OnPrepareRender();
	if (this->isActive)
	{
		if (mesh)
		{
			UpdateShaderVariable(commandList, &world);

			if (materialCount > 0)
			{
				for (int i = 0; i < materialCount; ++i)
				{
					if (materials[i])
					{
						if (materials[i]->GetShader())
						{
							materials[i]->GetShader()->Render(commandList, camera);
							materials[i]->UpdateShaderVariable(commandList);
						}
					}
					mesh->Render(commandList, i);
				}
			}
		}

		if (sibling)
		{
			sibling->Render(commandList, camera);
		}

		if (child)
		{
			child->Render(commandList, camera);
		}

	}
}

void Object::BuildMaterials(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
}

void Object::CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
}

void Object::UpdateShaderVariables(ID3D12GraphicsCommandList* commandList)
{
}

void Object::UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, XMFLOAT4X4* world)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(world)));
	commandList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);
}

void Object::UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, Material* material)
{
}

void Object::ReleaseShadervariables()
{
}

void Object::ReleaseUploadBuffers()
{
}

void Object::UpdateTransform(XMFLOAT4X4* parent)
{
}

SkinnedMesh* Object::FindSkinnedMesh(const char* skinnedMeshName)
{
	SkinnedMesh* skinnedMesh = nullptr;
	
	if (mesh && (mesh->GetType() & VERTEXT_BONE_INDEX_WEIGHT))
	{
		skinnedMesh = (SkinnedMesh*)mesh;
		if (!strncmp(skinnedMesh->GetMeshName(), skinnedMeshName, strlen(skinnedMeshName)))
		{
			return skinnedMesh;
		}
	}

	if (sibling)
	{
		if (skinnedMesh = sibling->FindSkinnedMesh(skinnedMeshName))
		{
			return(skinnedMesh);
		}
	}
	if (child)
	{
		if (skinnedMesh = child->FindSkinnedMesh(skinnedMeshName))
		{
			return(skinnedMesh);
		}
	}

	return nullptr;
}

void Object::FindAndSetSkinnedMesh(SkinnedMesh** skinnedMeshs, int* skinnedMeshCount)
{
	if (mesh && (mesh->GetType() & VERTEXT_BONE_INDEX_WEIGHT)) skinnedMeshs[(*skinnedMeshCount)++] = (SkinnedMesh*)mesh;

	if (sibling) sibling->FindAndSetSkinnedMesh(skinnedMeshs, skinnedMeshCount);
	if (child) child->FindAndSetSkinnedMesh(skinnedMeshs, skinnedMeshCount);
}

void Object::SetTrackAnimationSet(int animationTrack, int animationSet)
{
	if (skinnedAnimationController) skinnedAnimationController->SetTrackAnimationSet(animationTrack, animationSet);
}

void Object::SetTrackAnimationPosition(int animationTrack, float position)
{
	if (skinnedAnimationController) skinnedAnimationController->SetTrackPosition(animationTrack, position);
}

void Object::LoadMaterialsFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, Object* pParent, FILE* pInFile, Shader* pShader)
{
	char pstrToken[64] = { '\0' };
	int nMaterial = 0;
	UINT nReads = 0;

	materialCount = ReadIntegerFromFile(pInFile);

	materials = new Material * [materialCount];
	for (int i = 0; i < materialCount; i++) materials[i] = NULL;

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
			nReads = (UINT)::fread(&(pMaterial->albedoColor), sizeof(float), 4, pInFile);
		}
		else if (!strcmp(pstrToken, "<EmissiveColor>:"))
		{
			nReads = (UINT)::fread(&(pMaterial->emissiveColor), sizeof(float), 4, pInFile);
		}
		else if (!strcmp(pstrToken, "<SpecularColor>:"))
		{
			nReads = (UINT)::fread(&(pMaterial->specularColor), sizeof(float), 4, pInFile);
		}
		else if (!strcmp(pstrToken, "<Glossiness>:"))
		{
			nReads = (UINT)::fread(&(pMaterial->glossiness), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Smoothness>:"))
		{
			nReads = (UINT)::fread(&(pMaterial->smoothness), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Metallic>:"))
		{
			nReads = (UINT)::fread(&(pMaterial->specularHighlight), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<SpecularHighlight>:"))
		{
			nReads = (UINT)::fread(&(pMaterial->metallic), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<GlossyReflection>:"))
		{
			nReads = (UINT)::fread(&(pMaterial->glossyReflection), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<AlbedoMap>:"))
		{
			pMaterial->LoadTextureFromFile(pd3dDevice, pd3dCommandList, MATERIAL_ALBEDO_MAP, 3, pMaterial->textureNames[0], &(pMaterial->textures[0]), pParent, pInFile, pShader);
		}
		else if (!strcmp(pstrToken, "<SpecularMap>:"))
		{
			materials[nMaterial]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, MATERIAL_SPECULAR_MAP, 4, pMaterial->textureNames[1], &(pMaterial->textures[1]), pParent, pInFile, pShader);
		}
		else if (!strcmp(pstrToken, "<NormalMap>:"))
		{
			materials[nMaterial]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, MATERIAL_NORMAL_MAP, 5, pMaterial->textureNames[2], &(pMaterial->textures[2]), pParent, pInFile, pShader);
		}
		else if (!strcmp(pstrToken, "<MetallicMap>:"))
		{
			materials[nMaterial]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, MATERIAL_METALLIC_MAP, 6, pMaterial->textureNames[3], &(pMaterial->textures[3]), pParent, pInFile, pShader);
		}
		else if (!strcmp(pstrToken, "<EmissionMap>:"))
		{
			materials[nMaterial]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, MATERIAL_EMISSION_MAP, 7, pMaterial->textureNames[4], &(pMaterial->textures[4]), pParent, pInFile, pShader);
		}
		else if (!strcmp(pstrToken, "<DetailAlbedoMap>:"))
		{
			materials[nMaterial]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, MATERIAL_DETAIL_ALBEDO_MAP, 8, pMaterial->textureNames[5], &(pMaterial->textures[5]), pParent, pInFile, pShader);
		}
		else if (!strcmp(pstrToken, "<DetailNormalMap>:"))
		{
			materials[nMaterial]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, MATERIAL_DETAIL_NORMAL_MAP, 9, pMaterial->textureNames[6], &(pMaterial->textures[6]), pParent, pInFile, pShader);
		}
		else if (!strcmp(pstrToken, "</Materials>"))
		{
			break;
		}
	}
}

void Object::LoadAnimationFromFile(FILE* pInFile, ModelInfo* pLoadedModel)
{

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

			::ReadStringFromFile(pInFile, pGameObject->frameName);
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
			nReads = (UINT)::fread(&pGameObject->toParent, sizeof(float), 16, pInFile);
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
				}
			}
		}
		else if (!strcmp(pstrToken, "</Frame>"))
		{
			break;
		}
	}
	return pGameObject;
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
				pLoadedModel->modelRootObject = Object::LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, NULL, pInFile, pShader, &pLoadedModel->skinnedMeshCount);
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

	Object::PrintFrameInfo(pGameObject, NULL);
#endif


	return pLoadedModel;
}

void Object::PrintFrameInfo(Object* pGameObject, Object* pParent)
{
	TCHAR pstrDebug[256] = { 0 };

	_stprintf_s(pstrDebug, 256, _T("(Frame: %p) (Parent: %p)\n"), pGameObject, pParent);
	OutputDebugString(pstrDebug);

	if (pGameObject->sibling) Object::PrintFrameInfo(pGameObject->sibling, pParent);
	if (pGameObject->child) Object::PrintFrameInfo(pGameObject->child, pGameObject);
}

void Object::SetMesh(Mesh* mesh)
{
	SAFE_RELEASE(this->mesh);
	this->mesh = mesh;
	if (this->mesh)
	{
		this->mesh->AddRef();
	}
}

void Object::SetShader(Shader* shader)
{
}

void Object::SetShader(int materialCount, Shader* shader)
{
}

void Object::SetMaterial(int materialCount, Material* material)
{
}

void Object::SetChild(Object* child, bool referenceUpdate)
{
	if (child)
	{
		child->parent = this;
	}
	if (this->child)
	{
		if (child)
		{
			child->sibling = this->child->sibling;
			this->child->sibling = child;
		}
	}
	else
	{
		this->child = child;
	}
}

void Object::SetIsActive(bool active)
{
	this->isActive = active;
}

const bool Object::GetIsActive()
{
	return isActive;
}

const char* Object::GetFrameName()
{
	return this->frameName;
}

const XMFLOAT3& Object::GetPosition()
{
	return XMFLOAT3(world._41, world._42, world._43);
}

void Object::SetPosition(float x, float y, float z)
{
	toParent._41 = x;
	toParent._42 = y;
	toParent._43 = z;

	UpdateTransform(nullptr);
}

void Object::SetPosition(const XMFLOAT3& position)
{
	toParent._41 = position.x;
	toParent._42 = position.y;
	toParent._43 = position.z;

	UpdateTransform(nullptr);
}

const XMFLOAT3& Object::GetLook()
{
	return XMFLOAT3(world._31, world._32, world._33);
}

void Object::SetLook(float x, float y, float z)
{
	toParent._31 = x;
	toParent._32 = y;
	toParent._33 = z;
}

void Object::SetLook(const XMFLOAT3& look)
{
	toParent._31 = look.x;
	toParent._32 = look.y;
	toParent._33 = look.z;
}

const XMFLOAT3& Object::GetUp()
{
	return XMFLOAT3(world._21, world._22, world._23);
}

void Object::SetUp(float x, float y, float z)
{
	toParent._21 = x;
	toParent._22 = y;
	toParent._23 = z;
}

void Object::SetUp(const XMFLOAT3& up)
{
	toParent._21 = up.x;
	toParent._22 = up.y;
	toParent._23 = up.z;
}

const XMFLOAT3& Object::GetRight()
{
	return XMFLOAT3(world._11, world._12, world._13);
}

void Object::SetRight(float x, float y, float z)
{
	toParent._11 = x;
	toParent._12 = y;
	toParent._13 = z;
}

void Object::SetRight(const XMFLOAT3& right)
{
	toParent._11 = right.x;
	toParent._12 = right.y;
	toParent._13 = right.z;
}

const XMFLOAT3& Object::GetScale()
{
	return XMFLOAT3(toParent._11, toParent._22, toParent._33);
}

void Object::SetScale(float x, float y, float z)
{
	scale = { x,y,z };
	XMMATRIX mtxScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	toParent = Matrix4x4::Multiply(mtxScale, toParent);
	UpdateTransform(NULL);
}

void Object::SetScale(const XMFLOAT3& scale)
{
	this->scale = scale;
	XMMATRIX mtxScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	toParent = Matrix4x4::Multiply(mtxScale, toParent);
	UpdateTransform(NULL);
}

void Object::MoveForward(float distance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Look = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Look, distance);
	Object::SetPosition(xmf3Position);
}

void Object::MoveStrafe(float distance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Right, distance);
	Object::SetPosition(xmf3Position);
}

void Object::MoveUp(float distance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Up, distance);
	Object::SetPosition(xmf3Position);
}

void Object::Rotate(float pitch, float yaw, float roll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(pitch), XMConvertToRadians(yaw), XMConvertToRadians(roll));
	toParent = Matrix4x4::Multiply(mtxRotate, toParent);

	UpdateTransform(NULL);
}

void Object::Rotate(XMFLOAT3* axis, float angle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(axis), XMConvertToRadians(angle));
	toParent = Matrix4x4::Multiply(mtxRotate, toParent);

	UpdateTransform(NULL);
}

void Object::Rotate(XMFLOAT4* quarernion)
{
	XMMATRIX mtxRotate = XMMatrixRotationQuaternion(XMLoadFloat4(quarernion));
	toParent = Matrix4x4::Multiply(mtxRotate, toParent);

	UpdateTransform(NULL);
}

Object* Object::GetParent()
{
	return this->parent;
}

Object* Object::FindFrame(char* frameName)
{
	Object* frameObject = nullptr;
	if (!strncmp(this->frameName, frameName, strlen(frameName)))
	{
		return this;
	}

	if (sibling)
	{
		if (frameObject = sibling->FindFrame(frameName))
		{
			return frameObject;
		}
	}

	if (child)
	{
		if (frameObject = child->FindFrame(frameName))
		{
			return frameObject;
		}
	}

	return nullptr;
}

Texture* Object::FindReplicatedTexture(_TCHAR* textureName)
{
	for (int i = 0; i < materialCount; i++)
	{
		if (materials[i])
		{
			for (int j = 0; j < materials[i]->GetTextureCount(); j++)
			{
				if (materials[i]->GetTextures(i))
				{
					if (!_tcsncmp(materials[i]->GetTextureNames(j), textureName, _tcslen(textureName)))
					{
						return(materials[i]->GetTextures(j));
					}
				}
			}
		}
	}
	Texture* pTexture = nullptr;
	if (sibling) if (pTexture = sibling->FindReplicatedTexture(textureName)) return pTexture;
	if (child) if (pTexture = child->FindReplicatedTexture(textureName)) return pTexture;

	return nullptr;
}

UINT Object::GetMeshType()
{
	return (mesh) ? mesh->GetType() : 0x00;
}

const XMFLOAT4X4& Object::GetToParent()
{
	return this->toParent;
}

void Object::SetToParent(const XMFLOAT4X4& matrix)
{
	this->toParent = matrix;
}
