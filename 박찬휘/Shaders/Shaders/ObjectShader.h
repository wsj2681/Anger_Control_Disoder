#pragma once
class ObjectShader : public StandardShader {
public:
	ObjectShader() = default;
	ObjectShader(const ObjectShader&) = delete;
	ObjectShader& operator=(const ObjectShader&) = delete;
	~ObjectShader() = default;
protected:
	GameObject** object{ nullptr };
	int nObject{ 0 };
public:
	virtual void BuildObject(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* rootSignature, void* context = nullptr);
	virtual void AnimateObject(float fTimeElapsed);
	virtual void ReleaseObject();

	virtual void ReleaseUploadBuffer();

	virtual void Render(ID3D12GraphicsCommandList* commandList, Camera* camera);
};