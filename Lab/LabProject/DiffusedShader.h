#pragma once
#include "Shader.h"
class DiffusedShader : public Shader
{
public:
	DiffusedShader();
	virtual ~DiffusedShader();

	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();
};

