#include "framework.h"
#include "Texture.h"

const TextureType& Texture::GetTextureType() const
{
	return type;
}

const int& Texture::GetNTextures() const
{
	return nTextures;
}

ID3D12Resource** Texture::GetTextures() const
{
	return textures;
}

//void Texture::SetRootArg(const int& index, const SRVROOTARGINFO& srvRootArgInfo)
//{
//	rootArgInfos[index] = srvRootArgInfo;
//}

void Texture::SetGpuDescriptorHandle(int index, D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorNextHandle)
{
	rootArgInfos[index].srvGpuDescHandle = srvGPUDescriptorNextHandle;
}

void Texture::SetRootParameterIndex(int index, int rootParamIndex)
{
	rootArgInfos[index].rootParamIndex = rootParamIndex;
}

void Texture::SetSampler(const int& index, const D3D12_GPU_DESCRIPTOR_HANDLE& samplerGpuDescHandle)
{
	samplerDescHandles[index] = samplerGpuDescHandle;
}

void Texture::UpdateShaderVariables(ID3D12GraphicsCommandList* commandList)
{
	if (type == TextureType::RESOURCE_TEXTURE2D_ARRAY)
		commandList->SetGraphicsRootDescriptorTable(rootArgInfos[0].rootParamIndex, rootArgInfos[0].srvGpuDescHandle);
	else
	{
		for (int i = 0; i < nTextures; ++i)
			commandList->SetGraphicsRootDescriptorTable(rootArgInfos[1].rootParamIndex, rootArgInfos[1].srvGpuDescHandle);
	}
}

void Texture::UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, int index)
{
	commandList->SetGraphicsRootDescriptorTable(rootArgInfos[index].rootParamIndex, rootArgInfos[index].srvGpuDescHandle);
}

void Texture::ReleaseShaderVariables()
{

}

void Texture::LoadTextureFromFile(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, wchar_t* fName, unsigned int index, bool isDDSFile)
{
	if (isDDSFile)
		textures[index] = ::CreateTextureResourceFromDDSFile(device, commandList, fName, &(textureUploadBuffers[index]), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	else
		textures[index] = ::CreateTextureResourceFromWICFile(device, commandList, fName, &(textureUploadBuffers[index]), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void Texture::ReleaseUploadBuffers()
{
	if (textureUploadBuffers)
	{
		for (int i = 0; i < nTextures; ++i)
			SAFE_RELEASE(textureUploadBuffers[i]);
		delete[] textureUploadBuffers;
		textureUploadBuffers = nullptr;
	}
}
