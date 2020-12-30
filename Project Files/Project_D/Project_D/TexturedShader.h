#pragma once

class TexturedShader : public Shader {
public:
	TexturedShader() = default;
	TexturedShader(const TexturedShader&) = delete;
	TexturedShader& operator=(const TexturedShader&) = delete;
	virtual ~TexturedShader() = 0;
public:
	// functions
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob * *vertexShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob * *pixelShaderBlob);

	virtual void CreateShader(ID3D12Device * device, ID3D12RootSignature * graphicsRootSignature);
};