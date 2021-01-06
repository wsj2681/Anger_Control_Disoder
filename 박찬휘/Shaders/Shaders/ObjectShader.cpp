#include "framework.h"
#include "Shader.h"
#include "StandardShader.h"
#include "ObjectShader.h"

void ObjectShader::BuildObject(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* rootSignature, void* context)
{
	//nObject = n;
	//object = new GameObject * [nObject];
}

void ObjectShader::AnimateObject(float fTimeElapsed)
{
}

void ObjectShader::ReleaseObject()
{
}

void ObjectShader::ReleaseUploadBuffer()
{
}

void ObjectShader::Render(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
}
