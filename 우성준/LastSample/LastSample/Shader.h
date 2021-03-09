#pragma once

struct CB_OBJECT_INFO
{
	XMFLOAT4X4 world;
};


class Camera;

class Shader
{
public:
	Shader() = default;
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	virtual ~Shader() = default;

protected:

	ID3D12PipelineState* pipelineState = nullptr;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{};

	ID3DBlob* vertexShaderBlob = nullptr;
	ID3DBlob* pixelShaderBlob = nullptr;
public:

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();

	D3D12_SHADER_BYTECODE CompileShaderFromFile(WCHAR* pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob** ppd3dShaderBlob);
	D3D12_SHADER_BYTECODE ReadCompiledShaderFromFile(WCHAR* pszFileName, ID3DBlob** ppd3dShaderBlob = NULL);

	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) { }
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList) { }
	virtual void ReleaseShaderVariables() { }

	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, FbxAMatrix* pfbxf4x4World) { }

	virtual void OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList, int nPipelineState = 0);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera);

	virtual void ReleaseUploadBuffers() { }

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature) { }
	virtual void AnimateObjects(float fTimeElapsed) { }
	virtual void ReleaseObjects() { }
};

class CubeShader : public Shader
{
public:
	CubeShader() = default;
	virtual ~CubeShader() = default;

public:

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();

};