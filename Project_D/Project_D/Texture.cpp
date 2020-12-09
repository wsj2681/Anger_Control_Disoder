//#include "framework.h"
//#include "Texture.h"
//
//const int& Texture::GetTextures() const
//{
//    return nTextures;
//}
//
//const UINT& Texture::GetTextureType() const
//{
//    return nTextureType;
//}
//
//const UINT& Texture::GetTextureType(const int& index) const
//{
//    return resourceTypes[index];
//}
//
//ID3D12Resource* Texture::GetResource(int index)
//{
//    return textures[index];
//}
//
////const D3D12_SHADER_RESOURCE_VIEW_DESC& Texture::GetShaderResourceViewDesc(int index)
////{
////    ID3D12Resource* shaderResource{ GetResource(index) };
////    D3D12_RESOURCE_DESC resourceDesc{ shaderResource->GetDesc() };
////
////    D3D12_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
////    ::ZeroMemory(&shaderResourceViewDesc, sizeof(D3D12_SHADER_RESOURCE_VIEW_DESC));
////
////    nTextureType = GetTextureType(index);
////    switch (nTextureType)
////    {
////    case 0x01: // Resource Texture 2D
////    case 0x02: // Resource Texture2D Array
////        shaderResourceViewDesc.Format = resourceDesc.Format;
////        shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
////        shaderResourceViewDesc.Texture2D.MipLevels = -1;
////        shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
////        shaderResourceViewDesc.Texture2D.PlaneSlice = 0;
////        shaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.f;
////        break;
////    case 0x03: // Resource Texture2DArray
////        shaderResourceViewDesc.Format = resourceDesc.Format;
////        shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
////        shaderResourceViewDesc.Texture2DArray.MipLevels = -1;
////        shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
////        shaderResourceViewDesc.Texture2DArray.PlaneSlice = 0;
////        shaderResourceViewDesc.Texture2DArray.ResourceMinLODClamp = 0.f;
////        shaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
////        shaderResourceViewDesc.Texture2DArray.ArraySize = resourceDesc.DepthOrArraySize;
////        break;
////    case 0x04: // Resource Texture Cube
////        shaderResourceViewDesc.Format = resourceDesc.Format;
////        shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
////        shaderResourceViewDesc.TextureCube.MipLevels = 1;
////        shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
////        shaderResourceViewDesc.TextureCube.ResourceMinLODClamp = 0.f;
////        break;
////    //case 0x05: // Resource Buffer
////    //    shaderResourceViewDesc.Format = bufferFormats[index];
////    }
////    return shaderResourceViewDesc;
////}
//
//void Texture::AddRef()
//{
//    ++nRefers;
//}
//
//void Texture::Release()
//{
//    if (--nRefers <= 0) delete this;
//}