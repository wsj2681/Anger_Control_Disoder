#include "framework.h"
#include "Shader.h"

Shader::~Shader()
{
	SAFE_RELEASE(graphicsRootSignature);
	SAFE_RELEASE(descHeap);
	if (pipelineStates)
		for (int i = 0; i < nPipelineState; ++i)
			SAFE_RELEASE(pipelineStates[i]);
}

D3D12_INPUT_LAYOUT_DESC Shader::CreateInputLayout()
{
	D3D12_INPUT_LAYOUT_DESC desc;
	desc.pInputElementDescs = nullptr;
	desc.NumElements = 0;

	return desc;
}

D3D12_RASTERIZER_DESC Shader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC desc;
	desc.AntialiasedLineEnable = false;
	desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	desc.CullMode = D3D12_CULL_MODE_BACK;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.f;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D12_FILL_MODE_SOLID;
	desc.ForcedSampleCount = 0;
	desc.FrontCounterClockwise = false;
	desc.MultisampleEnable = false;
	desc.SlopeScaledDepthBias = 0.f;

	return desc;
}

D3D12_DEPTH_STENCIL_DESC Shader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC desc;
	desc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	desc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	desc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	desc.DepthEnable = true;
	desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	desc.StencilEnable = false;
	desc.StencilReadMask = 0;
	desc.StencilWriteMask = 0;

	return desc;
}

D3D12_BLEND_DESC Shader::CreateBlendState()
{
	D3D12_BLEND_DESC desc;
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = false;
	desc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	desc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	desc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	desc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	desc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	desc.RenderTarget[0].LogicOpEnable = false;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	desc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	desc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;

	return desc;
}

D3D12_SHADER_BYTECODE Shader::CreateShaderByteCode(ID3DBlob** shaderBlob)
{
	D3D12_SHADER_BYTECODE shaderByteCode;
	shaderByteCode.BytecodeLength = 0;
	shaderByteCode.pShaderBytecode = nullptr;

	return shaderByteCode;
}

void Shader::CreateDescriptorHeaps(ID3D12Device* device, int constantBufferViews, int shaderResourceViews)
{
	ASSERT(device, "Shader::CreateDescriptorHeaps device is nullptr");
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.NumDescriptors = constantBufferViews + shaderResourceViews;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	device->CreateDescriptorHeap(&desc, __uuidof(ID3D12DescriptorHeap), (void**)&descHeap);

	cbvCPUDescStartHandle = descHeap->GetCPUDescriptorHandleForHeapStart();
	cbvGPUDescStartHandle = descHeap->GetGPUDescriptorHandleForHeapStart();
	srvCPUDescStartHandle.ptr = static_cast<unsigned long long>(::descriptorIncrementSize) + constantBufferViews;
	srvGPUDescStartHandle.ptr = static_cast<unsigned long long>(::descriptorIncrementSize) + constantBufferViews;

	srvCPUDescNextHandle = srvCPUDescStartHandle;
	srvGPUDescNextHandle = srvGPUDescStartHandle;
}

void Shader::CreateConstantBufferViews(ID3D12Device* device, int constantBufferViews, ID3D12Resource* constantBuffers, UINT stride)
{
	ASSERT(device, "Shader::CreateConstantBufferViews device is nullptr");
	// ASSERT(constantBuffers, "Shader::CreateConstantBufferViews constantBuffer is nullptr");
	D3D12_GPU_VIRTUAL_ADDRESS gpuVirAddr = constantBuffers->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
	desc.SizeInBytes = stride;
	for (int i = 0; i < constantBufferViews; ++i)
	{
		desc.BufferLocation = gpuVirAddr + (static_cast<unsigned long long>(stride) * i);
		D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandle;
		cpuDescHandle.ptr = cbvCPUDescStartHandle.ptr + (static_cast<unsigned long long>(::descriptorIncrementSize) * i);
		device->CreateConstantBufferView(&desc, cpuDescHandle);
	}
}

void Shader::CreateShaderResourceViews(ID3D12Device* device, Texture* texture, UINT descriptorHeapIndex, UINT rootParameterStartIndex)
{
	ASSERT(device, "Shader::CreateShaderResourceViews device is nullptr");
	// ASSERT(texture, "Shader::CreateShaderResourceViews texture is nullptr");

	srvCPUDescNextHandle.ptr = (static_cast<unsigned long long>(::descriptorIncrementSize) * descriptorHeapIndex);
	srvGPUDescNextHandle.ptr = (static_cast<unsigned long long>(::descriptorIncrementSize) * descriptorHeapIndex);

	int textures = texture->GetTextures();
	UINT textureType = texture->GetTextureType();

	for (int i = 0; i < textures; ++i)
	{
		ID3D12Resource* shaderResource = texture->GetResource(i);
		if (shaderResource)
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC desc = texture->GetShaderResourceViewDesc(i);
			device->CreateShaderResourceView(shaderResource, &desc, srvCPUDescNextHandle);
			srvCPUDescNextHandle.ptr += ::descriptorIncrementSize;
			texture->SetGpuDescriptorHandle(i, srvGPUDescNextHandle);
			srvGPUDescNextHandle.ptr += ::descriptorIncrementSize;
		}
	}
	
	int rootParam = texture->GetRootParameters();
	for (int i = 0; i < rootParam; ++i) texture->SetRootParameterIndex(i, rootParameterStartIndex + i);
}

void Shader::CreateShader(ID3D12Device* device, ID3D12RootSignature* graphicsRootSignature)
{

}

D3D12_SHADER_BYTECODE Shader::CompileShaderFromFile(const wchar_t* fName, LPCSTR shaderName, LPCSTR shaderProfile, ID3DBlob** shaderBlob)
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
}
