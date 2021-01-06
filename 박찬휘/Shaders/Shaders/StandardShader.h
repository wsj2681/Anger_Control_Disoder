#pragma once
class StandardShader : public Shader {
public:
	StandardShader() = default;
	StandardShader(const StandardShader&) = delete;
	StandardShader& operator=(const StandardShader&) = delete;
	virtual ~StandardShader();
public:
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();
	virtual D3D12_SHADER_BYTECODE CreateGeometryShader();
};