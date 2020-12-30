#pragma once

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

class Shader {
public:
	Shader() = default;
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	virtual ~Shader() = 0;
protected:
	// variables
	ID3D12PipelineState**		pipelineStates			{ nullptr };
	int							nPipelineStates			{ 0 };
	ID3D12DescriptorHeap*		descriptorHeap			{ nullptr };
	ID3D12RootSignature*		graphicsRootSignature	{ nullptr };
	// Renderer					renderer				{};
	D3D12_CPU_DESCRIPTOR_HANDLE	cbvCPUDescriptorStartHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE	cbvGPUDescriptorStartHandle{};
	D3D12_CPU_DESCRIPTOR_HANDLE	srvCPUDescriptorStartHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE	srvGPUDescriptorStartHandle{};

	D3D12_CPU_DESCRIPTOR_HANDLE	srvCPUDescriptorNextHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE	srvGPUDescriptorNextHandle{};
public:
	// functions
	// create states
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
	virtual D3D12_BLEND_DESC CreateBlendState();

	// create shader
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** shaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateHullShader(ID3DBlob** hullShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateDomainShader(ID3DBlob** domainShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateGeometryShader(ID3DBlob** shaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shaderBlob);
	
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