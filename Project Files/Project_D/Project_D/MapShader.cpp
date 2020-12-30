#include "framework.h"
#include "Shader.h"
#include "MapShader.h"

D3D12_INPUT_LAYOUT_DESC MapShader::CreateInputLayout()
{
	UINT nInputElementDescs = 4;
	D3D12_INPUT_ELEMENT_DESC* inputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	inputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	inputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	inputElementDescs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	inputElementDescs[3] = { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = nInputElementDescs;

	return inputLayoutDesc;
}

D3D12_RASTERIZER_DESC MapShader::CreateRasterizerState()
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

D3D12_SHADER_BYTECODE MapShader::CreateVertexShader(ID3DBlob** vertexShaderBlob)
{
	return Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSTerrainTessellation", "vs_5_1", vertexShaderBlob);
}

D3D12_SHADER_BYTECODE MapShader::CreateHullShader(ID3DBlob** hullShaderBlob)
{
	return Shader::CompileShaderFromFile(L"Shaders.hlsl", "HSTerrainTessellation", "hs_5_1", hullShaderBlob);
}

D3D12_SHADER_BYTECODE MapShader::CreateDomainShader(ID3DBlob** domainShaderBlob)
{
	return Shader::CompileShaderFromFile(L"Shaders.hlsl", "DSTerrainTessellation", "ds_5_1", domainShaderBlob);
}

D3D12_SHADER_BYTECODE MapShader::CreatePixelShader(ID3DBlob** pixelShaderBlob)
{
	return Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSTerrainTessellation", "ps_5_1", pixelShaderBlob);
}

void MapShader::CreateShader(ID3D12Device* device, ID3D12RootSignature* graphicsRootSignature)
{
	ASSERT(device, "Shader::Render device is nullptr");
	ASSERT(graphicsRootSignature, "Shader::Render graphicsRootSignature is nullptr");

	this->graphicsRootSignature = graphicsRootSignature;

	nPipelineStates = 2;
	pipelineStates = new ID3D12PipelineState * [nPipelineStates];

	ID3DBlob* vertexShaderBlob{ nullptr }, * pixelShaderBlob{ nullptr }, * geometryShaderBlob{ nullptr }, * hullShaderBlob{ nullptr }, * domainShaderBlob{ nullptr };

	// create and init graphics pipeline state desc
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelinStateDesc;
	::ZeroMemory(&graphicsPipelinStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	graphicsPipelinStateDesc.pRootSignature = graphicsRootSignature;
	graphicsPipelinStateDesc.VS = CreateVertexShader(&vertexShaderBlob);
	graphicsPipelinStateDesc.PS = CreatePixelShader(&pixelShaderBlob);
	graphicsPipelinStateDesc.HS = CreateHullShader(&hullShaderBlob);
	graphicsPipelinStateDesc.DS = CreateDomainShader(&domainShaderBlob);
	graphicsPipelinStateDesc.RasterizerState = CreateRasterizerState();
	graphicsPipelinStateDesc.BlendState = CreateBlendState();
	graphicsPipelinStateDesc.DepthStencilState = CreateDepthStencilState();
	graphicsPipelinStateDesc.InputLayout = CreateInputLayout();
	graphicsPipelinStateDesc.SampleMask = UINT_MAX;
	graphicsPipelinStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
	graphicsPipelinStateDesc.NumRenderTargets = 1;
	graphicsPipelinStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	graphicsPipelinStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	graphicsPipelinStateDesc.SampleDesc.Count = 1;
	graphicsPipelinStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	HR(device->CreateGraphicsPipelineState(&graphicsPipelinStateDesc, __uuidof(ID3D12PipelineState), (void**)&pipelineStates[0]));

	graphicsPipelinStateDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	HR(device->CreateGraphicsPipelineState(&graphicsPipelinStateDesc, __uuidof(ID3D12PipelineState), (void**)&pipelineStates[1]));

	SAFE_RELEASE(vertexShaderBlob);
	SAFE_RELEASE(pixelShaderBlob);
	SAFE_RELEASE(hullShaderBlob);
	SAFE_RELEASE(domainShaderBlob);

	if (graphicsPipelinStateDesc.InputLayout.pInputElementDescs) delete[] graphicsPipelinStateDesc.InputLayout.pInputElementDescs;
}

void MapShader::Render(ID3D12GraphicsCommandList* commandList)
{
	ASSERT(commandList, "Shader::Render commandList is nullptr");

	if (graphicsRootSignature) commandList->SetGraphicsRootSignature(graphicsRootSignature);
	if (pipelineStates) commandList->SetPipelineState(pipelineStates[0]);
	if (descriptorHeap)commandList->SetDescriptorHeaps(1, &descriptorHeap);

	// UpdateShaderVariables(commandList);
}