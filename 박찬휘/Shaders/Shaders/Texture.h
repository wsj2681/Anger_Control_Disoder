struct SRVROOTARGINFO {
	int rootParamIndex{ 0 };
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuDescHandle{};
};

enum class TextureType {
	RESOURCE_TEXTURE2D,
	RESOURCE_TEXTURE2D_ARRAY,
	RESOURCE_TEXTURE2DARRAY,
	RESOURCE_TEXTURE_CUBE,
	RESOURCE_BUFFER
};

class Texture final {
public:
	Texture() = default;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	~Texture() = default;
protected:
	TextureType type{ TextureType::RESOURCE_TEXTURE2D };
	int nTextures{ 0 };
	ID3D12Resource** textures{ nullptr };
	ID3D12Resource** textureUploadBuffers{ nullptr };

	int nSamplers{ 0 };
	D3D12_GPU_DESCRIPTOR_HANDLE* samplerDescHandles{ nullptr };
public:
	SRVROOTARGINFO* rootArgInfos{ nullptr };
public:
	const TextureType& GetTextureType() const;
	const int& GetNTextures() const;
	ID3D12Resource** GetTextures() const;
	//--------------------------------------------------------------------------
	//int GetTextures() { return 0; }}
	ID3D12Resource* GetResource(int index) { return nullptr; }
	D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(int index) { return D3D12_SHADER_RESOURCE_VIEW_DESC(); }
	int GetRootParameters() { return 0; }
	//--------------------------------------------------------------------------

	//void SetRootArg(const int& index, const SRVROOTARGINFO& srvRootArgInfo);
	void SetGpuDescriptorHandle(int index, D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorNextHandle);
	void SetRootParameterIndex(int index, int rootParamIndex);
	void SetSampler(const int& index, const D3D12_GPU_DESCRIPTOR_HANDLE& samplerGpuDescHandle);

	void UpdateShaderVariables(ID3D12GraphicsCommandList* commandList);
	void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, int index);
	void ReleaseShaderVariables();

	void LoadTextureFromFile(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, wchar_t* fName, unsigned int index, bool isDDSFile = true);

	void ReleaseUploadBuffers();

	
	

	
};