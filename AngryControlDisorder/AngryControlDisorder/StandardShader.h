#pragma once
#include "Shader.h"
class StandardShader : public Shader
{
public:

	StandardShader() = default;
	virtual ~StandardShader() = default;

public:

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();

};

