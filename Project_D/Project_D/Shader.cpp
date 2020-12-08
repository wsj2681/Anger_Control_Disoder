#include "framework.h"
#include "Renderer.h"
#include "Shader.h"

Shader::~Shader()
{
	SAFE_RELEASE(graphicsRootSignature);
	SAFE_RELEASE(descriptorHeap);
	if (pipelineStates)
		for (int i = 0; i < nPipelineState; ++i)
			SAFE_RELEASE(pipelineStates[i]);
}

D3D12_INPUT_LAYOUT_DESC Shader::CreateInputLayout()
{
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;

	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;

	return inputLayoutDesc;
}

D3D12_RASTERIZER_DESC Shader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC rasterizerDesc;

	::ZeroMemory(&rasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.f;
	rasterizerDesc.SlopeScaledDepthBias = 0.f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return rasterizerDesc;
}

D3D12_DEPTH_STENCIL_DESC Shader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc;

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.StencilReadMask = 0x00;
	depthStencilDesc.StencilWriteMask = 0x00;

	return depthStencilDesc;
}

D3D12_BLEND_DESC Shader::CreateBlendState()
{
	D3D12_BLEND_DESC blendDesc;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = false;
	blendDesc.RenderTarget[0].LogicOpEnable = false;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return blendDesc;
}

D3D12_SHADER_BYTECODE Shader::CreateVertexShader(ID3DBlob** vertexShaderBlob)
{
	D3D12_SHADER_BYTECODE vertexShader;

	vertexShader.BytecodeLength = 0;
	vertexShader.pShaderBytecode = nullptr;

	return vertexShader;
}

D3D12_SHADER_BYTECODE Shader::CreateHullShader(ID3DBlob** hullShaderBlob)
{
	D3D12_SHADER_BYTECODE hullShader;

	hullShader.BytecodeLength = 0;
	hullShader.pShaderBytecode = nullptr;

	return hullShader;
}

D3D12_SHADER_BYTECODE Shader::CreateDomainShader(ID3DBlob** domainShaderBlob)
{
	D3D12_SHADER_BYTECODE domainShader;

	domainShader.BytecodeLength = 0;
	domainShader.pShaderBytecode = nullptr;

	return domainShader;
}

D3D12_SHADER_BYTECODE Shader::CreateGeometryShader(ID3DBlob** geometryShaderBlob)
{
	D3D12_SHADER_BYTECODE geometryShader;

	geometryShader.BytecodeLength = 0;
	geometryShader.pShaderBytecode = nullptr;

	return geometryShader;
}

D3D12_SHADER_BYTECODE Shader::CreatePixelShader(ID3DBlob** pixelShaderBlob)
{
	D3D12_SHADER_BYTECODE pixelShader;

	pixelShader.BytecodeLength = 0;
	pixelShader.pShaderBytecode = nullptr;

	return pixelShader;
}

void Shader::CreateDescriptorHeaps(ID3D12Device* device, int constantBufferViews, int shaderResourceViews)
{
	ASSERT(device, "Shader::Render device is nullptr");

	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc;
	descriptorHeapDesc.NumDescriptors = constantBufferViews + shaderResourceViews;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descriptorHeapDesc.NodeMask = 0;

	HR(device->CreateDescriptorHeap(&descriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&descriptorHeap));

	cbvCPUDescriptorStartHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	cbvGPUDescriptorStartHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	srvCPUDescriptorStartHandle.ptr += static_cast<unsigned long long>(::descriptorIncrementSize) + constantBufferViews;
	srvGPUDescriptorStartHandle.ptr += static_cast<unsigned long long>(::descriptorIncrementSize) + constantBufferViews;

	srvCPUDescriptorNextHandle = srvCPUDescriptorStartHandle;
	srvGPUDescriptorNextHandle = srvGPUDescriptorStartHandle;
}

void Shader::CreateConstantBufferViews(ID3D12Device* device, int constantBufferViews, ID3D12Resource* constantBuffers, UINT stride)
{
	ASSERT(device, "Shader::Render device is nullptr");
	// ASSERT(constantBuffers, "Shader::Render constantBuffers is nullptr");

	D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress = constantBuffers->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.SizeInBytes = stride;
	for (int i = 0; i < constantBufferViews; ++i)
	{
		cbvDesc.BufferLocation = gpuVirtualAddress + (static_cast<unsigned long long>(stride) * i);
		D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle;
		cpuDescriptorHandle;
		cpuDescriptorHandle.ptr = cbvCPUDescriptorStartHandle.ptr + (static_cast<unsigned long long>(::descriptorIncrementSize) * i);
		device->CreateConstantBufferView(&cbvDesc, cpuDescriptorHandle);
	}
}

void Shader::CreateShaderResourceViews(ID3D12Device* device, Texture* texture, UINT descriptorHeapIndex, UINT rootParameterStartIndex)
{
	ASSERT(device, "Shader::Render device is nullptr");
	// ASSERT(texture, "Shader::Render texture is nullptr");

	srvCPUDescriptorNextHandle.ptr += (static_cast<unsigned long long>(::descriptorIncrementSize) * descriptorHeapIndex);
	srvGPUDescriptorNextHandle.ptr += (static_cast<unsigned long long>(::descriptorIncrementSize) * descriptorHeapIndex);

	int textures = texture->GetTextures();
	UINT textureType = texture->GetTextureType();

	for (int i = 0; i < textures; ++i)
	{
		ID3D12Resource* shaderResource = texture->GetResource(i);
		if (shaderResource)
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = texture->GetShaderResourceViewDesc(i);
			device->CreateShaderResourceView(shaderResource, &shaderResourceViewDesc, srvCPUDescriptorNextHandle);
			srvCPUDescriptorNextHandle.ptr += ::descriptorIncrementSize;
			texture->SetGpuDescriptorHandle(i, srvGPUDescriptorNextHandle);
			srvGPUDescriptorNextHandle.ptr += ::descriptorIncrementSize;
		}
	}
	int rootParameters = texture->GetRootParameters();
	for (int i = 0; i < rootParameters; ++i) texture->SetRootParameterIndex(i, rootParameterStartIndex + i);
}

void Shader::CreateShader(ID3D12Device* device, ID3D12RootSignature* graphicsRootSignature)
{
	ASSERT(device, "Shader::Render device is nullptr");
	ASSERT(graphicsRootSignature, "Shader::Render graphicsRootSignature is nullptr");

	if (graphicsRootSignature) this->graphicsRootSignature = graphicsRootSignature;

	ID3DBlob* vertexShaderBlob{ nullptr }, * pixelShaderBlob{ nullptr }, * geometryShaderBlob{ nullptr }, * hullShaderBlob{ nullptr }, * domainShaderBlob{ nullptr };

	// create and init graphics pipeline state desc
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelinStateDesc;
	::ZeroMemory(&graphicsPipelinStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	graphicsPipelinStateDesc.pRootSignature = graphicsRootSignature;
	graphicsPipelinStateDesc.VS = CreateVertexShader(&vertexShaderBlob);
	graphicsPipelinStateDesc.PS = CreatePixelShader(&pixelShaderBlob);
	graphicsPipelinStateDesc.GS = CreateGeometryShader(&geometryShaderBlob);
	graphicsPipelinStateDesc.HS = CreateHullShader(&hullShaderBlob);
	graphicsPipelinStateDesc.DS = CreateDomainShader(&domainShaderBlob);
	graphicsPipelinStateDesc.RasterizerState = CreateRasterizerState();
	graphicsPipelinStateDesc.BlendState = CreateBlendState();
	graphicsPipelinStateDesc.DepthStencilState = CreateDepthStencilState();
	graphicsPipelinStateDesc.InputLayout = CreateInputLayout();
	graphicsPipelinStateDesc.SampleMask = UINT_MAX;
	graphicsPipelinStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipelinStateDesc.NumRenderTargets = 1;
	graphicsPipelinStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	graphicsPipelinStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	graphicsPipelinStateDesc.SampleDesc.Count = 1;
	graphicsPipelinStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	HR(device->CreateGraphicsPipelineState(&graphicsPipelinStateDesc, __uuidof(ID3D12PipelineState), (void**)&pipelineStates[0]));

	SAFE_RELEASE(vertexShaderBlob);
	SAFE_RELEASE(pixelShaderBlob);
	SAFE_RELEASE(geometryShaderBlob);
	SAFE_RELEASE(hullShaderBlob);
	SAFE_RELEASE(domainShaderBlob);

	if (graphicsPipelinStateDesc.InputLayout.pInputElementDescs) delete[] graphicsPipelinStateDesc.InputLayout.pInputElementDescs;
}

D3D12_SHADER_BYTECODE Shader::CompileShaderFromFile(WCHAR* fName, LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob** shaderBlob)
{
	return D3D12_SHADER_BYTECODE();
}

void Shader::BuildObjects(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
}

void Shader::AnimateObjects(float timeElapsed)
{
}

void Shader::ReleaseObjects()
{
}

void Shader::ReleaseUploadBuffer()
{
}

void Shader::Render(ID3D12GraphicsCommandList* commandList)
{
	ASSERT(commandList, "Shader::Render commandList is nullptr");

	if (graphicsRootSignature) commandList->SetGraphicsRootSignature(graphicsRootSignature);
	if (pipelineStates) commandList->SetPipelineState(pipelineStates[0]);
	if (descriptorHeap)commandList->SetDescriptorHeaps(1, &descriptorHeap);

	// UpdateShaderVariables(commandList);
}