#include "framework.h"
#include "Texture.h"
#include "Shader.h"


Texture::Texture(const int& Textures, const UINT& TextureType, const int& Samplers, const int& RootParameters)
{
	this->Textures = Textures;
	this->TextureType = TextureType;

	if (Textures > 0) {
		TextureUpLoadBuffer = new ID3D12Resource *[this->Textures];
		ResourceTextures = new ID3D12Resource *[this->Textures];

		for (int i = 0; i < this->Textures; i++)
			this->TextureUpLoadBuffer[i] = ResourceTextures[i] = NULL;

		this->strTexturename = new _TCHAR[this->Textures][64];
		for (int i = 0; i < this->Textures; i++)
			strTexturename[i][0] = '\0';

		this->SrvGpuDescriptorHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[this->Textures];
		for (int i = 0; i < this->Textures; i++)
			this->SrvGpuDescriptorHandles[i].ptr = NULL;

		this->ResourceTypes = new UINT[this->Textures];
		this->dxgiBufferFormats = new DXGI_FORMAT[this->Textures];
		this->BufferElements = new int[this->Textures];

	}
	this->RootParameters = RootParameters;
	if (RootParameters > 0) {
		this->RootParameterlndices = new UINT[RootParameters];
		for (int i = 0; i < RootParameters; i++)
			this->RootParameterlndices[i] - 1;
	}

	this->Samplers = Samplers;
	if (Samplers > 0)
		this->SampleGpuDescriptorHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[this->Samplers];


}


Texture::~Texture()
{
	if (this->Textures) {
		for (int i = 0; i < this->Textures; i++)
			if (this->ResourceTextures[i])
				ResourceTextures[i]->Release();
		delete[] ResourceTextures;
	}

	if (this->strTexturename)
		delete[] this->strTexturename;

	if (this->ResourceTypes)
		delete[] this->ResourceTypes;
	if (this->BufferElements)
		delete[] this->BufferElements;
	if (this->RootParameterlndices)
		delete[] this->RootParameterlndices;
	if (this->SrvGpuDescriptorHandles)
		delete[] this->SrvGpuDescriptorHandles;

	//ป๙วร
	if (this->SampleGpuDescriptorHandles)
		delete[] this->SampleGpuDescriptorHandles;
}


void Texture::AddRef() { this->References++; };
void Texture::Release() {
	if (--this->References <= 0)
		delete this;
}

// Shade Func()
void Texture::UpdateShaderVariable(ID3D12GraphicsCommandList* Commandlist, int parameterIndex, int TextureIndex) {
	ASSERT(Commandlist != nullptr, "Texture Class / UpdateShaderVariable / Commandlist/ must not be null ");
	Commandlist->SetGraphicsRootDescriptorTable(this->RootParameterlndices[parameterIndex], this->SrvGpuDescriptorHandles[TextureIndex]);

}
void Texture::UpdateShaderVariables(ID3D12GraphicsCommandList* Commandlist) {
	ASSERT(Commandlist != nullptr, "Texture Class / UpdateShaderVariable / Commandlist/ must not be null ");
	if (this->RootParameters == this->Textures) {
		for (int i = 0; i < this->RootParameters; i++) {
			if (this->SrvGpuDescriptorHandles[i].ptr && (this->RootParameterlndices[i] != -1))
				Commandlist->SetGraphicsRootDescriptorTable(this->RootParameterlndices[i], this->SrvGpuDescriptorHandles[i]);
		}
	}
	else {
		if (this->SrvGpuDescriptorHandles[0].ptr) {
			Commandlist->SetGraphicsRootDescriptorTable(this->RootParameterlndices[0], this->SrvGpuDescriptorHandles[0]);

		}
	}



}
void Texture::ReleaseShaderVariables() {

}

// TextureLoad
void Texture::LoadTextureFromDDSFile(ID3D12Device* Device, ID3D12GraphicsCommandList* Commandlist, wchar_t* FileName, const UINT& ResourceType, const UINT& index) {
	ASSERT(Commandlist != nullptr, "Texture Class / LoadTextureFromDDSFile / Commandlist / must not be null ");
	ASSERT(Device != nullptr, "Texture Class / LoadTextureFromDDSFile / Device / must not be null ");
	ASSERT(FileName != nullptr, "Texture Class / LoadTextureFromDDSFile / FileName / must not be null ");

	this->ResourceTypes[index] = ResourceType;
	this->ResourceTextures[index] = CreateTextureResourceFromDDSFile(Device, Commandlist, FileName, &this->TextureUpLoadBuffer[index], D3D12_RESOURCE_STATE_GENERIC_READ);

}

// Set Func()
void Texture::SetSampler(int index, const D3D12_GPU_DESCRIPTOR_HANDLE& SamplerHandle) {
	this->SampleGpuDescriptorHandles[index] = SamplerHandle;
}
void Texture::SetRootParameterIndex(int index, const UINT& RootParameterIndex) {

	this->RootParameterlndices[index] = RootParameterIndex;

}
void Texture::SetGpuDescriptorHandle(int index, const D3D12_GPU_DESCRIPTOR_HANDLE& SrvHandle) {

	this->SrvGpuDescriptorHandles[index] = SrvHandle;

}

// Get Func()
int Texture::GetRootParameters() {

	return this->RootParameters;

}
int Texture::GetTextures() {
	return this->Textures;
}
_TCHAR* Texture::GetTextureName(const int& index) {
	return  this->strTexturename[index];
}
ID3D12Resource* Texture::GetResource(const int& index) {
	return this->ResourceTextures[index];
}
const D3D12_GPU_DESCRIPTOR_HANDLE& Texture::GetGpuDescriptorHanle(int index) const {
	return this->SrvGpuDescriptorHandles[index];
}
int Texture::GetRootParameter(int index) {
	return this->RootParameterlndices[index];
}

const UINT& Texture::GetTextureType() const {
	return this->TextureType;
}
const UINT& Texture::GetResourceTextureType(int index) const {
	return this->ResourceTypes[index];
}
const DXGI_FORMAT& Texture::GetBufferFormat(int index) const {
	return this->dxgiBufferFormats[index];
}
int Texture::GetBufferElements(int index) {
	return this->BufferElements[index];
}

D3D12_SHADER_RESOURCE_VIEW_DESC Texture::GetShaderResourceViewDesc(int index) {
	ID3D12Resource* ShaderResource = GetResource(index);
	D3D12_RESOURCE_DESC ResourceDesc = ShaderResource->GetDesc();

	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	int newTextureType = GetResourceTextureType(index);
	//switch (newTextureType)
	//{
	//default:
	//	break;
	//}
	return(ShaderResourceViewDesc);
}

// Release
void Texture::ReleaseUploadBuffer() {
	if (this->TextureUpLoadBuffer)
	{
		for (int i = 0; i < this->Textures; i++)
			if (this->TextureUpLoadBuffer[i])
				this->TextureUpLoadBuffer[i]->Release();
		delete[] this->TextureUpLoadBuffer;
		this->TextureUpLoadBuffer = NULL;
	}
}