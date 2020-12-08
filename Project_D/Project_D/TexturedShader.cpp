#include "framework.h"
#include "Shader.h"
#include "TexturedShader.h"

D3D12_INPUT_LAYOUT_DESC TexturedShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* inputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	inputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	inputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = nInputElementDescs;

	return inputLayoutDesc;
}

D3D12_SHADER_BYTECODE TexturedShader::CreateVertexShader(ID3DBlob** vertexShaderBlob)
{
	return Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSTextured", "vs_5_1", vertexShaderBlob);
}

D3D12_SHADER_BYTECODE TexturedShader::CreatePixelShader(ID3DBlob** pixelShaderBlob)
{
	return Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSTextured", "vs_5_1", pixelShaderBlob);
}

void TexturedShader::CreateShader(ID3D12Device* device, ID3D12RootSignature* graphicsRootSignature)
{
	nPipelineStates = 1;
	pipelineStates = new ID3D12PipelineState*[nPipelineStates];

	Shader::CreateShader(device, graphicsRootSignature);
}