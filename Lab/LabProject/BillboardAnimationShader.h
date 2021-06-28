#pragma once
#include "Shader.h"

class Texture;
class Scene;

struct VS_CB_ANIMATION_INFO
{
	unsigned int keyFrame = 0;
};

class BillboardAnimationShader : public Shader
{
public:
	BillboardAnimationShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);
	virtual ~BillboardAnimationShader();

protected:

	Texture* m_pTexture = nullptr;

	bool active = true;

	ID3D12Resource* m_pd3dcbkeyFrameInfo = NULL;
	VS_CB_ANIMATION_INFO* m_pcbMappedkeyFrameInfo = NULL;

public:

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();
	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);

	//void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera);
	bool isActive() const { return this->active; }
	void SetActive(bool active) { this->active = active; }

};

