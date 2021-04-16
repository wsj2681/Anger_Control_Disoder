#pragma once
#include "StandardShader.h"

class SkinnedAnimationStandardShader : public StandardShader
{
public:
	SkinnedAnimationStandardShader();
	virtual ~SkinnedAnimationStandardShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
};