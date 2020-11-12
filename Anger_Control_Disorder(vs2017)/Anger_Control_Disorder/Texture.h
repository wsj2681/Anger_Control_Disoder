#pragma once;


class Texture
{
public:
	Texture() = default;
	Texture(const int& Textures, const UINT& TextureType, const int& Samplers, const int& RootParameters);
	Texture& operator=(const Texture&) = delete;
	~Texture();

private:
	int References = 0;

	UINT TextureType;

	//받아 올 텍스쳐를 위한변수
	int Textures = 0;
	_TCHAR(*strTexturename)[64] = NULL;
	ID3D12Resource** ResourceTextures = NULL;
	ID3D12Resource** TextureUpLoadBuffer;

	//리소스 타입
	UINT* ResourceTypes = NULL;

	// 포맷 버퍼
	DXGI_FORMAT* dxgiBufferFormats = NULL;
	int* BufferElements = NULL;

	int RootParameters = 0;
	UINT* RootParameterlndices = NULL;
	D3D12_GPU_DESCRIPTOR_HANDLE* SrvGpuDescriptorHandles = NULL;


	int Samplers = 0;
	D3D12_GPU_DESCRIPTOR_HANDLE* SampleGpuDescriptorHandles = NULL;

public:

	void AddRef();
	void Release();



	// Shade Func()
	void UpdateShaderVariable(ID3D12GraphicsCommandList* Commandlist, int parameterIndex, int TextureIndex);
	void UpdateShaderVariables(ID3D12GraphicsCommandList* Commandlist);
	void ReleaseShaderVariables();

	// TextureLoad
	void LoadTextureFromDDSFile(ID3D12Device* Device, ID3D12GraphicsCommandList* Commandlist, wchar_t* FileName, const UINT& ResourceType, const UINT& index);

	// Set Func()
	void SetSampler(int index, const D3D12_GPU_DESCRIPTOR_HANDLE& SamplerHandle);
	void SetRootParameterIndex(int index, const UINT& RootPArameterIndex);
	void SetGpuDescriptorHandle(int index, const D3D12_GPU_DESCRIPTOR_HANDLE& SrvHandle);

	// Get Func()
	int GetRootParameters();
	int GetTextures();
	_TCHAR* GetTextureName(const int& index);
	ID3D12Resource* GetResource(const int& index);
	const D3D12_GPU_DESCRIPTOR_HANDLE& GetGpuDescriptorHanle(int index) const;
	int GetRootParameter(int index);

	const UINT& GetTextureType() const;
	const UINT& GetResourceTextureType(int index) const;
	const DXGI_FORMAT& GetBufferFormat(int index) const;
	int GetBufferElements(int index);

	D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(int index);

	// Release
	void ReleaseUploadBuffer();


};

