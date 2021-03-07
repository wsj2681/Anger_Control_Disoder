#include "framework.h"
#include "Object.h"

Texture::Texture(int textureResources, UINT resourceType, int samplers, int rootParameters)
{
	textureType = resourceType;
	textureCount = textureResources;

	if (textureCount > 0)
	{
		textureUploadBuffer = new ID3D12Resource * [textureCount];
		textures = new ID3D12Resource * [textureCount];
		for (int i = 0; i < textureCount; ++i)
		{
			textureUploadBuffer[i] = textures[i] = nullptr;
		}

		srvGPUDescHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[textureCount];
		for (int i = 0; i < textureCount; ++i)
		{
			srvGPUDescHandles[i].ptr = 0;
		}
		this->resourceType = new UINT[textureCount];
		bufferFormats = new DXGI_FORMAT[textureCount];
		bufferElements = new int[textureCount];
	}

	this->rootParameterCount = rootParameters;
	if (rootParameterCount > 0)
	{
		rootParameterIndices = new UINT[rootParameterCount];
		for (int i = 0; i < rootParameterCount; ++i)
		{
			rootParameterIndices[i] = -1;
		}
	}

	this->samplers = samplers;
	if (this->samplers > 0)
	{
		samplerGPUDescHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[this->samplers];
	}

}

void Texture::SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle)
{
	samplerGPUDescHandles[nIndex] = d3dSamplerGpuDescriptorHandle;
}

void Texture::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, int nParameterIndex, int nTextureIndex)
{
	pd3dCommandList->SetGraphicsRootDescriptorTable(rootParameterIndices[nParameterIndex], srvGPUDescHandles[nTextureIndex]);
}

void Texture::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (rootParameterCount == textureCount)
	{
		for (int i = 0; i < rootParameterCount; ++i)
		{
			if (srvGPUDescHandles[i].ptr && (rootParameterIndices[i] != -1))
			{
				pd3dCommandList->SetGraphicsRootDescriptorTable(rootParameterIndices[i], srvGPUDescHandles[i]);
			}
		}
	}
	else
	{
		if (srvGPUDescHandles[0].ptr)
		{
			pd3dCommandList->SetGraphicsRootDescriptorTable(rootParameterIndices[0], srvGPUDescHandles[0]);
		}
	}
}

void Texture::ReleaseShaderVariables()
{
}

void Texture::LoadTextureFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nResourceType, UINT nIndex)
{
	resourceType[nIndex] = nResourceType;
	textures[nIndex] = ::CreateTextureResourceFromDDSFile(pd3dDevice, pd3dCommandList, pszFileName, &textureUploadBuffer[nIndex], D3D12_RESOURCE_STATE_GENERIC_READ);
}

void Texture::SetRootParameterIndex(int nIndex, UINT nRootParameterIndex)
{
	rootParameterIndices[nIndex] = nRootParameterIndex;
}

void Texture::SetGpuDescriptorHandle(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle)
{
	srvGPUDescHandles[nIndex] = d3dSrvGpuDescriptorHandle;
}

D3D12_SHADER_RESOURCE_VIEW_DESC Texture::GetShaderResourceViewDesc(int nIndex)
{
	ID3D12Resource* pShaderResource = GetResource(nIndex);
	D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();

	D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc;
	d3dShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	int nTextureType = GetTextureType(nIndex);
	switch (nTextureType)
	{
	case RESOURCE_TEXTURE2D: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 1)
	case RESOURCE_TEXTURE2D_ARRAY: //[]
		d3dShaderResourceViewDesc.Format = d3dResourceDesc.Format;
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		d3dShaderResourceViewDesc.Texture2D.MipLevels = -1;
		d3dShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.Texture2D.PlaneSlice = 0;
		d3dShaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		break;
	case RESOURCE_TEXTURE2DARRAY: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize != 1)
		d3dShaderResourceViewDesc.Format = d3dResourceDesc.Format;
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
		d3dShaderResourceViewDesc.Texture2DArray.MipLevels = -1;
		d3dShaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.Texture2DArray.PlaneSlice = 0;
		d3dShaderResourceViewDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
		d3dShaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
		d3dShaderResourceViewDesc.Texture2DArray.ArraySize = d3dResourceDesc.DepthOrArraySize;
		break;
	case RESOURCE_TEXTURE_CUBE: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 6)
		d3dShaderResourceViewDesc.Format = d3dResourceDesc.Format;
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		d3dShaderResourceViewDesc.TextureCube.MipLevels = 1;
		d3dShaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.TextureCube.ResourceMinLODClamp = 0.0f;
		break;
	case RESOURCE_BUFFER: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
		d3dShaderResourceViewDesc.Format = bufferFormats[nIndex];
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		d3dShaderResourceViewDesc.Buffer.FirstElement = 0;
		d3dShaderResourceViewDesc.Buffer.NumElements = bufferElements[nIndex];
		d3dShaderResourceViewDesc.Buffer.StructureByteStride = 0;
		d3dShaderResourceViewDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		break;
	}
	return(d3dShaderResourceViewDesc);
}

void Texture::ReleaseUploadBuffers()
{
}
