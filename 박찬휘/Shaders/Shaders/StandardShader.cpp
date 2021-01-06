#include "framework.h"
#include "Shader.h"
#include "StandardShader.h"

StandardShader::~StandardShader()
{
}

D3D12_INPUT_LAYOUT_DESC StandardShader::CreateInputLayout()
{
	UINT nInputElementDescs{ 1 };
	D3D12_INPUT_ELEMENT_DESC* inputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	inputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = nInputElementDescs;

	return inputLayoutDesc;
}

D3D12_DEPTH_STENCIL_DESC StandardShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC desc;
	desc.DepthEnable = false;
	desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;
	desc.StencilEnable = false;
	desc.StencilReadMask = 0xff;
	desc.StencilWriteMask = 0xff;
	desc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_INCR;
	desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	desc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_DECR;
	desc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	return desc;
}

D3D12_SHADER_BYTECODE StandardShader::CreateVertexShader()
{
	return Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSStandard", "vs_5_1", &vertexShaderBlob);
}

D3D12_SHADER_BYTECODE StandardShader::CreatePixelShader()
{
	return Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSStandard", "ps_5_1", &pixelShaderBlob);
}

D3D12_SHADER_BYTECODE StandardShader::CreateGeometryShader()
{
	return Shader::CompileShaderFromFile(L"Shaders.hlsl", "GSStandard", "gs_5_1", &geometryShaderBlob);
}
