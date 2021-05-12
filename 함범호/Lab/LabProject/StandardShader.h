#pragma once
#include "Shader.h"
class StandardShader : public Shader
{
public:
	StandardShader();
	virtual ~StandardShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();
};