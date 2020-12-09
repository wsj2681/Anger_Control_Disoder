#pragma once

class MapShader : public Shader {
public:
	MapShader() = default;
	MapShader(const MapShader&) = delete;
	MapShader& operator=(const MapShader&) = delete;
	~MapShader() = default;
public:
	// functions
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** vertexShaderBlob);
	/*virtual D3D12_SHADER_BYTECODE CreateHullShader(ID3DBlob * *hullShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateDomainShader(ID3DBlob** domainShaderBlob);*/
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** pixelShaderBlob);

	virtual void CreateShader(ID3D12Device* device, ID3D12RootSignature* graphicsRootSignature);

	virtual void Render(ID3D12GraphicsCommandList* commandList);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, XMFLOAT4X4* world);
};