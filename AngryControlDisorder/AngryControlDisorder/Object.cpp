#include "framework.h"
#include "Object.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"


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
