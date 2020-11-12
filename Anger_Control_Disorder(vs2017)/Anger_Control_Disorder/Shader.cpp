#include "framework.h"
#include "Shader.h"

void Shader::Release()
{
	if (this->PipelineState)
		this->PipelineState->Release();
	if (this->CBVandSRVDescriptorHeap)
		this->CBVandSRVDescriptorHeap->Release();
}

D3D12_SHADER_BYTECODE Shader::CreateVertexShader()
{
	D3D12_SHADER_BYTECODE ShaderByteCode;
	ShaderByteCode.BytecodeLength = 0;
	ShaderByteCode.pShaderBytecode = nullptr;

	return ShaderByteCode;
}

D3D12_SHADER_BYTECODE Shader::CreateVertexShader(ID3DBlob ** ShaderBlob)
{
	D3D12_SHADER_BYTECODE ShaderByteCode;
	ShaderByteCode.BytecodeLength = 0;
	ShaderByteCode.pShaderBytecode = nullptr;

	return ShaderByteCode;
}

D3D12_SHADER_BYTECODE Shader::CreatePixelShader()
{
	D3D12_SHADER_BYTECODE ShaderByteCode;
	ShaderByteCode.BytecodeLength = 0;
	ShaderByteCode.pShaderBytecode = nullptr;

	return ShaderByteCode;
}

D3D12_SHADER_BYTECODE Shader::CreatePixelShader(ID3DBlob ** ShaderBlob)
{
	D3D12_SHADER_BYTECODE ShaderByteCode;
	ShaderByteCode.BytecodeLength = 0;
	ShaderByteCode.pShaderBytecode = nullptr;

	return ShaderByteCode;
}

D3D12_SHADER_BYTECODE Shader::CreateGeometryShader()
{
	D3D12_SHADER_BYTECODE ShaderByteCode;
	ShaderByteCode.BytecodeLength = 0;
	ShaderByteCode.pShaderBytecode = nullptr;

	return ShaderByteCode;
}

D3D12_INPUT_LAYOUT_DESC Shader::InputLayoutDesc()
{
	D3D12_INPUT_LAYOUT_DESC InputLayOutDesc;
	InputLayOutDesc.pInputElementDescs = nullptr;
	InputLayOutDesc.NumElements = 0;

	return InputLayOutDesc;
}

D3D12_RASTERIZER_DESC Shader::RasterizerStateDesc()
{
	D3D12_RASTERIZER_DESC RasterizerDesc;
	::ZeroMemory(&RasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	RasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	RasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	RasterizerDesc.FrontCounterClockwise = false;
	RasterizerDesc.DepthBias = 0;
	RasterizerDesc.DepthBiasClamp = 0.f;
	RasterizerDesc.SlopeScaledDepthBias = 0.f;
	RasterizerDesc.DepthClipEnable = true;
	RasterizerDesc.MultisampleEnable = false;
	RasterizerDesc.AntialiasedLineEnable = false;
	RasterizerDesc.ForcedSampleCount = 0;
	RasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;


	return RasterizerDesc;
}

D3D12_BLEND_DESC Shader::BlendStateDesc()
{
	//블렌드는 타겟이 8개까지 있습니다. 필요시에 추가합니다.
	D3D12_BLEND_DESC BlendDesc;
	::ZeroMemory(&BlendDesc, sizeof(D3D12_BLEND_DESC));
	BlendDesc.AlphaToCoverageEnable = false;
	BlendDesc.IndependentBlendEnable = false;
	BlendDesc.RenderTarget[0].BlendEnable = false;
	BlendDesc.RenderTarget[0].LogicOpEnable = false;
	BlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


	return BlendDesc;
}

D3D12_DEPTH_STENCIL_DESC Shader::DepthStencilStateDesc()
{
	//깊이 스텐실은 깊이 검사와 스텐실 검사를 통해 마스킹을 할 수 있습니다.
	D3D12_DEPTH_STENCIL_DESC DepthStencilDesc;
	::ZeroMemory(&DepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	DepthStencilDesc.DepthEnable = true;
	DepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	DepthStencilDesc.StencilEnable = false;
	DepthStencilDesc.StencilReadMask = 0x00;
	DepthStencilDesc.StencilWriteMask = 0x00;
	DepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	DepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

	return DepthStencilDesc;
}

void Shader::CreatePipelineState(ID3D12Device * Device, ID3D12RootSignature * GraphicsRootSignature, D3D12_PRIMITIVE_TOPOLOGY_TYPE PrimitiveTopology)
{
	ID3DBlob *VertexShaderBlob = nullptr;
	ID3DBlob *PixelShaderBlob = nullptr;
	ID3DBlob *GeometryShaderBlob = nullptr;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc;
	::ZeroMemory(&PipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	PipelineStateDesc.pRootSignature = GraphicsRootSignature;
	PipelineStateDesc.VS = CreateVertexShader(&VertexShaderBlob);
	PipelineStateDesc.PS = CreatePixelShader(&PixelShaderBlob);
	PipelineStateDesc.GS = CreateGeometryShader();
	PipelineStateDesc.RasterizerState = RasterizerStateDesc();
	PipelineStateDesc.BlendState = BlendStateDesc();
	PipelineStateDesc.DepthStencilState = DepthStencilStateDesc();
	PipelineStateDesc.InputLayout = InputLayoutDesc();
	PipelineStateDesc.SampleMask = UINT_MAX;
	PipelineStateDesc.PrimitiveTopologyType = PrimitiveTopology;
	PipelineStateDesc.NumRenderTargets = 1;
	PipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	PipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	PipelineStateDesc.SampleDesc.Count = 0;
	PipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;


	//TODO : error = arg is invalied
	HRESULT hResult = Device->CreateGraphicsPipelineState(&PipelineStateDesc, __uuidof(ID3D12PipelineState), (void **)&PipelineStates[0]);

	if (VertexShaderBlob) VertexShaderBlob->Release();
	if (PixelShaderBlob) PixelShaderBlob->Release();
	if (GeometryShaderBlob) GeometryShaderBlob->Release();
	if (PipelineStateDesc.InputLayout.pInputElementDescs) 
		delete[] PipelineStateDesc.InputLayout.pInputElementDescs;
}

void Shader::CreateShader(ID3D12Device * Device, ID3D12RootSignature * GraphicsRootSignature, D3D12_PRIMITIVE_TOPOLOGY_TYPE PrimitiveTopology)
{
	this->PipelineStateCount = 1;
	this->PipelineStates = new ID3D12PipelineState*[this->PipelineStateCount];
	this->CreatePipelineState(Device, GraphicsRootSignature, PrimitiveTopology);
}

void Shader::CreateShaderVariables()
{
}

void Shader::UpdateShaderVariables()
{
}

void Shader::ReleaseShaderVariables()
{
}

D3D12_SHADER_BYTECODE Shader::CompileShaderFromFile(WCHAR * FileName, LPCSTR ShaderName, LPCSTR ShaderProfile, ID3DBlob ** ShaderBlobs)
{
	int compileFlag = 0;
	::D3DCompileFromFile(FileName, nullptr, nullptr, ShaderName, ShaderProfile, compileFlag, 0, ShaderBlobs, nullptr);

	D3D12_SHADER_BYTECODE ShaderByteCode;
	ShaderByteCode.BytecodeLength = (*ShaderBlobs)->GetBufferSize();
	ShaderByteCode.pShaderBytecode = (*ShaderBlobs)->GetBufferPointer();

	return ShaderByteCode;
}

void Shader::CreateCbvSrvDescriptorHeaps(ID3D12Device * Device, int ConstantBufferViews, int ShaderResourceViews)
{
	D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc;
	DescriptorHeapDesc.NumDescriptors = ConstantBufferViews + ShaderResourceViews;
	DescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	DescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DescriptorHeapDesc.NodeMask = 0;

	Device->CreateDescriptorHeap(&DescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&this->CBVandSRVDescriptorHeap);

	this->CbvCPUDescriptorHandle = this->CBVandSRVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	this->CbvGPUDescriptorHandle = this->CBVandSRVDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	this->SrvCPUDescriptorHandle.ptr = this->CbvCPUDescriptorHandle.ptr + (::gnCbvSrvDescriptorIncrementSize* ConstantBufferViews);
	this->SrvGPUDescriptorHandle.ptr = this->CbvGPUDescriptorHandle.ptr + (::gnCbvSrvDescriptorIncrementSize* ConstantBufferViews);

	this->SrvCPUDescriptorNextHandle = this->SrvCPUDescriptorHandle;
	this->SrvGPUDescriptorNextHandle = this->SrvGPUDescriptorHandle;
}

void Shader::CreateConstantBufferViews(ID3D12Device * Device, int ConstantBufferViews, ID3D12Resource * ConstantBuffers, UINT Stride)
{
	D3D12_GPU_VIRTUAL_ADDRESS GpuVirtualAddress = ConstantBuffers->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;
	
	CBVDesc.SizeInBytes = Stride;
	
	for (int j = 0; j < ConstantBufferViews; j++)
	{
		CBVDesc.BufferLocation = GpuVirtualAddress + (Stride * j);
		
		D3D12_CPU_DESCRIPTOR_HANDLE CbvCPUDescriptorHandle{};
		CbvCPUDescriptorHandle.ptr = CbvCPUDescriptorHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * j);
		Device->CreateConstantBufferView(&CBVDesc, CbvCPUDescriptorHandle);
	}
}

const D3D12_CPU_DESCRIPTOR_HANDLE & Shader::GetCPUDescriptorHandleForHeapStart() const
{
	return this->CBVandSRVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
}

const D3D12_GPU_DESCRIPTOR_HANDLE & Shader::GetGPUDescriptorHandleForHeapStart() const
{
	return this->CBVandSRVDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
}

const D3D12_CPU_DESCRIPTOR_HANDLE & Shader::GetCPUCbvDescriptorStartHandle() const
{
	return this->CbvCPUDescriptorHandle;
}

const D3D12_GPU_DESCRIPTOR_HANDLE & Shader::GetGPUCbvDescriptorStartHandle() const
{
	return this->CbvGPUDescriptorHandle;
}

const D3D12_CPU_DESCRIPTOR_HANDLE & Shader::GetCPUSrvDescriptorStartHandle() const
{
	return this->SrvCPUDescriptorHandle;
}

const D3D12_GPU_DESCRIPTOR_HANDLE & Shader::GetGPUSrvDescriptorStartHandle() const
{
	return this->SrvGPUDescriptorHandle;
}

ID3D12RootSignature * Shader::CreateGraphicsRootSignature(ID3D12Device * Device)
{
	return nullptr;
}

const ID3D12RootSignature * Shader::GetGraphicsRootSignature() const
{
	return this->GraphicsRootSignature;
}

void Shader::UpdateShader(ID3D12GraphicsCommandList * CommandList)
{
	if (this->PipelineState)
		CommandList->SetPipelineState(this->PipelineState);
	if (this->CBVandSRVDescriptorHeap)
		CommandList->SetDescriptorHeaps(1, &this->CBVandSRVDescriptorHeap);

	this->UpdateShaderVariables();
}
