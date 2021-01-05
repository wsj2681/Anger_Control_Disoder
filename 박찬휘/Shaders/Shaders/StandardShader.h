#pragma once
class StandardShader : public Shader {
public:
	StandardShader() = default;
	StandardShader(const StandardShader&) = delete;
	StandardShader& operator=(const StandardShader&) = delete;
	virtual ~StandardShader();
protected:
	GameObject** gameObject{ nullptr };
	int nGameObject{ 0 };
public:
	virtual void BuildObject(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, LoadedModelInfo* modelInfo, void* context = nullptr);
};