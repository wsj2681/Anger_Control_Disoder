#include "framework.h"
#include "Material.h"
#include "Object.h"
#include "Texture.h"
#include "Shader.h"
#include "Scene.h"
#include "StandardShader.h"


Shader* Material::skinnedAnimationShader = nullptr;
Shader* Material::standardShader = nullptr;

Material::Material()
{
}

Material::Material(int textures)
{
}

Material::~Material()
{
}

void Material::AddRef()
{
	this->references++;
}

void Material::Release()
{
	if (--references <= 0)
	{
		delete this;
	}
}

void Material::SetShader(Shader* shader)
{
	SAFE_RELEASE(this->shader);
	this->shader = shader;
	if (this->shader)
	{
		shader->AddRef();
	}
}

Shader* Material::GetShader()
{
	return this->shader;
}

_TCHAR* Material::GetTextureNames(int nIndex)
{
	return textureNames[nIndex];
}

Texture* Material::GetTextures(int nIndex)
{
	return textures[nIndex];
}

int Material::GetTextureCount()
{
	return this->textureCount;
}

void Material::SetMaterialType(UINT type)
{
	this->type |= type;
}

void Material::SetTexture(Texture* texture, UINT textureCount)
{

}

void Material::UpdateShaderVariable(ID3D12GraphicsCommandList* commandList)
{
}

void Material::ReleaseUploadBuffers()
{
}

void Material::SetStandardShader()
{
	Material::SetShader(standardShader);
}

void Material::SetSkinnedAnimationShader()
{
	Material::SetShader(skinnedAnimationShader);
}

void Material::LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nType, UINT nRootParameter, _TCHAR* pwstrTextureName, Texture** ppTexture, Object* pParent, FILE* pInFile, Shader* pShader)
{
	char pstrTextureName[64] = { '\0' };

	BYTE nStrLength = ::ReadStringFromFile(pInFile, pstrTextureName);

	bool bDuplicated = false;
	if (strcmp(pstrTextureName, "null"))
	{
		SetMaterialType(nType);

		char pstrFilePath[64] = { '\0' };
		strcpy_s(pstrFilePath, 64, "Model/Textures/");

		bDuplicated = (pstrTextureName[0] == '@');
		strcpy_s(pstrFilePath + 15, 64 - 15, (bDuplicated) ? (pstrTextureName + 1) : pstrTextureName);
		strcpy_s(pstrFilePath + 15 + ((bDuplicated) ? (nStrLength - 1) : nStrLength), 64 - 15 - ((bDuplicated) ? (nStrLength - 1) : nStrLength), ".dds");

		size_t nConverted = 0;
		mbstowcs_s(&nConverted, pwstrTextureName, 64, pstrFilePath, _TRUNCATE);

		if (!bDuplicated)
		{
			*ppTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
			(*ppTexture)->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pwstrTextureName, 0, true);
			if (*ppTexture) (*ppTexture)->AddRef();

			Scene::CreateShaderResourceViews(pd3dDevice, *ppTexture, nRootParameter, false);
		}
		else
		{
			if (pParent)
			{
				while (pParent)
				{
					if (!pParent->GetParent()) break;
					pParent = pParent->GetParent();
				}
				Object* pRootGameObject = pParent;
				*ppTexture = pRootGameObject->FindReplicatedTexture(pwstrTextureName);
				if (*ppTexture) (*ppTexture)->AddRef();
			}
		}
	}
}

void Material::PrepareShaders(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
}