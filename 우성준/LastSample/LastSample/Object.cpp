#include "framework.h"
#include "Object.h"
#include "Shader.h"
#include "Mesh.h"

Texture::Texture(int textures, UINT textureType, int samplers)
{
	this->texturType = textureType;
	this->textureCount = textures;

	if (textures > 0)
	{
		textureUploadBuffers = new ID3D12Resource * [this->textureCount];
		this->textures = new ID3D12Resource * [this->textureCount];
	}

	this->samplers = samplers;
	if(samplers > 0)
	{
		samplerGpuDescHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[this->samplers];
	}
}

void Texture::SetRootArgument(int nIndex, UINT nRootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dsrvGpuDescriptorHandle)
{
	rootArgumentInfos[nIndex].m_nRootParameterIndex = nRootParameterIndex;
	rootArgumentInfos[nIndex].m_d3dSrvGpuDescriptorHandle = d3dsrvGpuDescriptorHandle;
}

void Texture::SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle)
{
	samplerGpuDescHandles[nIndex] = d3dSamplerGpuDescriptorHandle;
}

void Texture::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (texturType == RESOURCE_TEXTURE2D_ARRAY)
	{
		pd3dCommandList->SetGraphicsRootDescriptorTable(rootArgumentInfos[0].m_nRootParameterIndex, rootArgumentInfos[0].m_d3dSrvGpuDescriptorHandle);
	}
	else
	{
		for (int i = 0; i < textureCount; ++i)
		{
			pd3dCommandList->SetGraphicsRootDescriptorTable(rootArgumentInfos[i].m_nRootParameterIndex, rootArgumentInfos[i].m_d3dSrvGpuDescriptorHandle);
		}
	}
}

void Texture::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, int nIndex)
{
	pd3dCommandList->SetGraphicsRootDescriptorTable(rootArgumentInfos[nIndex].m_nRootParameterIndex, rootArgumentInfos[nIndex].m_d3dSrvGpuDescriptorHandle);
}

void Texture::ReleaseShaderVariables()
{
}

void Texture::LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nIndex)
{
	textures[nIndex] = ::CreateTextureResourceFromDDSFile(pd3dDevice, pd3dCommandList, pszFileName, &textureUploadBuffers[nIndex], D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void Texture::LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* pszFileName, UINT nIndex)
{
	textures[nIndex] = ::CreateTextureResourceFromDDSFile(pd3dDevice, pd3dCommandList, (wchar_t*)pszFileName, &textureUploadBuffers[nIndex], D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

int Texture::GetTextureCount()
{
	return this->textureCount;
}

ID3D12Resource* Texture::GetTexture(int index)
{
	return this->textures[index];
}

UINT Texture::GetTextureType()
{
	return this->texturType;
}

void Material::SetAlbedo(XMFLOAT4 xmf4Albedo)
{
	this->albedo = xmf4Albedo;
}

void Material::SetReflection(UINT nReflection)
{
	reflection = nReflection;
}

void Material::SetTexture(Texture* pTexture)
{
	texture = pTexture;
}

void Material::SetShader(Shader* pShader)
{
	shader = pShader;
}

void Material::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (texture)
		texture->UpdateShaderVariables(pd3dCommandList);
}

void Material::ReleaseShaderVariables()
{
	
}

void Material::ReleaseUploadBuffers()
{
}

void Object::SetMesh(Mesh* mesh)
{
}

void Object::SetShader(Shader* shader)
{
}

void Object::Render(ID3D12GraphicsCommandList* commandList)
{
	if (mesh)
	{
		if (shader)
		{
			shader->Render(commandList, nullptr);
		}
		mesh->Render(commandList);
	}
}
