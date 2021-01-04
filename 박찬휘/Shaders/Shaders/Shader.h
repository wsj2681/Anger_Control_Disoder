// temporary class
//TODO : Camera
class Camera;

//TODO : Texture
class Texture {
public:
	Texture() = default;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	~Texture() = default;
public:
	int GetTextures() { return 0; }
	UINT GetTextureType() { return 0; }
	ID3D12Resource* GetResource(int index) { return nullptr; }
	D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(int index) { return D3D12_SHADER_RESOURCE_VIEW_DESC(); }
	int GetRootParameters() { return 0; }

	void SetGpuDescriptorHandle(int index, D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorNextHandle) {}
	void SetRootParameterIndex(int index, int rootParameterIndex) {}
};

class Shader
{
public:
	Shader() = default;
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	virtual ~Shader();
protected:
	ID3D12PipelineState** pipelineStates{ nullptr };
	int nPipelineState{ 0 };
	ID3D12DescriptorHeap* descHeap{ nullptr };
	ID3D12RootSignature* graphicsRootSignature{ nullptr };

	D3D12_CPU_DESCRIPTOR_HANDLE cbvCPUDescStartHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE cbvGPUDescStartHandle{};
	D3D12_CPU_DESCRIPTOR_HANDLE srvCPUDescStartHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescStartHandle{};

	D3D12_CPU_DESCRIPTOR_HANDLE srvCPUDescNextHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescNextHandle{};
public:
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
	virtual D3D12_BLEND_DESC CreateBlendState();

	virtual D3D12_SHADER_BYTECODE CreateShaderByteCode(ID3DBlob** shaderBlob);

	virtual void CreateDescriptorHeaps(ID3D12Device* device, int constantBufferViews, int shaderResourceViews);
	virtual void CreateConstantBufferViews(ID3D12Device* device, int constantBufferViews, ID3D12Resource* constantBuffers, UINT stride);
	virtual void CreateShaderResourceViews(ID3D12Device* device, Texture* texture, UINT descriptorHeapIndex, UINT rootParameterStartIndex);
	virtual void CreateShader(ID3D12Device* device, ID3D12RootSignature* graphicsRootSignature);

	D3D12_SHADER_BYTECODE CompileShaderFromFile(const wchar_t* fName, LPCSTR shaderName, LPCSTR shaderProfile, ID3DBlob** shaderBlob);

	virtual void BuildObjects(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	virtual void AnimateObjects(float timeElapsed);
	virtual void ReleaseObjects();
	virtual void ReleaseUploadBuffer();

	virtual void Render(ID3D12GraphicsCommandList* commandList);
};