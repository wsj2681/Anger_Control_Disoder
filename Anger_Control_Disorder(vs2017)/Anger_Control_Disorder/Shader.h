#pragma once
class Shader
{
public:
	Shader() = default;
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader) = delete;
	~Shader() = default;

private:

protected:
	ID3D12PipelineState* PipelineState = nullptr;
	ID3D12DescriptorHeap* CBVandSRVDescriptorHeap = nullptr;
	ID3DBlob* VsBlob = nullptr;
	ID3DBlob* PsBlob = nullptr;

	ID3D12PipelineState** PipelineStates;
	int PipelineStateCount = 0;

	ID3D12RootSignature* GraphicsRootSignature = nullptr;

	D3D12_CPU_DESCRIPTOR_HANDLE CbvCPUDescriptorHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE CbvGPUDescriptorHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE SrvCPUDescriptorHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE SrvGPUDescriptorHandle;

	//D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc;

	D3D12_CPU_DESCRIPTOR_HANDLE SrvCPUDescriptorNextHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE SrvGPUDescriptorNextHandle;

public:
	virtual void Release();


	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateGeometryShader();

	virtual D3D12_INPUT_LAYOUT_DESC InputLayoutDesc();
	virtual D3D12_RASTERIZER_DESC RasterizerStateDesc();
	virtual D3D12_BLEND_DESC BlendStateDesc();
	virtual D3D12_DEPTH_STENCIL_DESC DepthStencilStateDesc();

	virtual void CreatePipelineState(ID3D12Device *Device, ID3D12RootSignature *GraphicsRootSignature, D3D12_PRIMITIVE_TOPOLOGY_TYPE PrimitiveTopology);
	virtual void CreateShader(ID3D12Device *Device, ID3D12RootSignature *GraphicsRootSignature, D3D12_PRIMITIVE_TOPOLOGY_TYPE PrimitiveTopology);
	virtual void CreateShaderVariables();
	virtual void UpdateShaderVariables();
	virtual void ReleaseShaderVariables();

	D3D12_SHADER_BYTECODE CompileShaderFromFile(WCHAR *FileName, LPCSTR ShaderName, LPCSTR ShaderProfile, ID3DBlob **ShaderBlobs);

	void CreateCbvSrvDescriptorHeaps(ID3D12Device *Device, int ConstantBufferViews, int ShaderResourceViews);
	void CreateConstantBufferViews(ID3D12Device *Device, int ConstantBufferViews, ID3D12Resource *ConstantBuffers, UINT Stride);
	
	//TODO : TEXTURE
	//void CreateShaderResourceViews(ID3D12Device* Device, CTexture* Texture, UINT DescriptorHeapIndex, UINT RootParameterStartIndex);
	//void CreateShaderResourceView(ID3D12Device* Device, CTexture* Texture, int Index);


	const D3D12_CPU_DESCRIPTOR_HANDLE& GetCPUDescriptorHandleForHeapStart()const;
	const D3D12_GPU_DESCRIPTOR_HANDLE& GetGPUDescriptorHandleForHeapStart()const;

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetCPUCbvDescriptorStartHandle()const;
	const D3D12_GPU_DESCRIPTOR_HANDLE& GetGPUCbvDescriptorStartHandle()const;
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetCPUSrvDescriptorStartHandle()const;
	const D3D12_GPU_DESCRIPTOR_HANDLE& GetGPUSrvDescriptorStartHandle()const;

	virtual ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* Device);
	const ID3D12RootSignature* GetGraphicsRootSignature() const;

	void UpdateShader(ID3D12GraphicsCommandList* CommandList);

};

