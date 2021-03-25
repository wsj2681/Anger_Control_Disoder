#include "framework.h"
#include "Texture.h"

Texture::Texture(int nTextureResources, UINT nResourceType, int nSamplers)
	:textureCount(nTextureResources), textureType(nResourceType), samplers(nSamplers)
{
	if (textureCount > 0)
	{
		rootArgumentInfos = new SRVROOTARGUMENTINFO[textureCount];
		textureUploadBuffers = new ID3D12Resource * [textureCount];
		textures = new ID3D12Resource * [textureCount];
	}

	if (samplers > 0)
	{
		samplerGpuDescriptorHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[samplers];
	}
}

Texture::~Texture()
{
	if (textures)
	{
		for (int i = 0; i < textureCount; i++)
		{
			SAFE_RELEASE(textures[i]);
		}
		delete[] textures;
	}

	if (rootArgumentInfos)
	{
		delete[] rootArgumentInfos;
	}

	if (samplerGpuDescriptorHandles)
	{
		delete[] samplerGpuDescriptorHandles;
	}
}

void Texture::AddRef()
{
	this->references++;
}

void Texture::Release()
{
	if (--references <= 0)
	{
		delete this;
	}
}

void Texture::SetRootArgument(int nIndex, UINT nRootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dsrvGpuDescriptorHandle)
{
	rootArgumentInfos[nIndex].rootParameterIndex = nRootParameterIndex;
	rootArgumentInfos[nIndex].srvGpuDescriptorHandle = d3dsrvGpuDescriptorHandle;
}

void Texture::SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle)
{
	samplerGpuDescriptorHandles[nIndex] = d3dSamplerGpuDescriptorHandle;
}

void Texture::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (textureType == RESOURCE_TEXTURE2D_ARRAY)
	{
		pd3dCommandList->SetGraphicsRootDescriptorTable(rootArgumentInfos[0].rootParameterIndex, rootArgumentInfos[0].srvGpuDescriptorHandle);
	}
	else
	{
		for (int i = 0; i < textureCount; i++)
		{
			pd3dCommandList->SetGraphicsRootDescriptorTable(rootArgumentInfos[i].rootParameterIndex, rootArgumentInfos[i].srvGpuDescriptorHandle);
		}
	}
}

void Texture::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, int nIndex)
{
	pd3dCommandList->SetGraphicsRootDescriptorTable(rootArgumentInfos[nIndex].rootParameterIndex, rootArgumentInfos[nIndex].srvGpuDescriptorHandle);
}

void Texture::ReleaseShaderVariables()
{
	if (textureUploadBuffers)
	{
		for (int i = 0; i < textureCount; i++)
		{
			SAFE_RELEASE(textureUploadBuffers[i]);
		}
		delete[] textureUploadBuffers;

		textureUploadBuffers = nullptr;;
	}
}

void Texture::LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nIndex, bool bIsDDSFile)
{
	if (bIsDDSFile)
		textures[nIndex] = ::CreateTextureResourceFromDDSFile(pd3dDevice, pd3dCommandList, pszFileName, &(textureUploadBuffers[nIndex]), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	else
		textures[nIndex] = ::CreateTextureResourceFromWICFile(pd3dDevice, pd3dCommandList, pszFileName, &(textureUploadBuffers[nIndex]), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

}

int Texture::GetTextureCount()
{
	return this->textureCount;
}

ID3D12Resource* Texture::GetTexture(int nIndex)
{
	return textures[nIndex];
}

UINT Texture::GetTextureType()
{
	return this->textureType;
}

void Texture::ReleaseUploadBuffers()
{
	if (textureUploadBuffers)
	{
		for (int i = 0; i < textureCount; i++)
		{
			SAFE_RELEASE(textureUploadBuffers[i]);
		}

		delete[] textureUploadBuffers;

		textureUploadBuffers = nullptr;
	}
}
