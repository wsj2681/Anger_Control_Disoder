#pragma once

class Shader
{
public:
	Shader() = default;
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	virtual ~Shader();
protected:
	ID3D12PipelineState* pipelineState{ nullptr };

	ID3D12DescriptorHeap* descHeap{ nullptr };

	ID3DBlob* vertexShaderBlob{ nullptr };
	ID3DBlob* pixelShaderBlob{ nullptr };
	ID3DBlob* geometryShaderBlob{ nullptr };

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
	D3D12_SHADER_BYTECODE ReadCompiledShaderFromFile(const wchar_t* fName, ID3DBlob** shaderBlob = nullptr);

	virtual void BuildObjects(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, LoadedModelInfo* modelInfo, void* context = nullptr);
	virtual void AnimateObjects(float timeElapsed);
	virtual void ReleaseObjects();
	virtual void ReleaseUploadBuffer();

	//virtual void CreateShaderVariables(ID3D12Device* device, ID3D12CommandList* commandList);
	//virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* commandList);
	//virtual void ReleaseShaderVariables();

	virtual void Render(ID3D12GraphicsCommandList* commandList);
};