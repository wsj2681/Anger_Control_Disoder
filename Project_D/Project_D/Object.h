#pragma once

class Mesh;
class MapShader;

class Object
{
public:
	Object() = default;
	//Object(int nMeshes);
	Object(const Object&) = delete;
	Object& operator=(const Object&) = delete;
	virtual ~Object();
protected:
	XMFLOAT4X4 world{ Matrix4x4::Identity() };
	//Mesh** meshes{ nullptr };
	//int nMeshes{ 0 };
	//D3D12_GPU_DESCRIPTOR_HANDLE gpuDescriptorHandle{};
	//ID3D12Resource* object{ nullptr };
	//TODO : Material ±¸Çö
	// Material* material{ nullptr };
	Mesh* mesh{ nullptr };
	MapShader* shader{ nullptr };
public:
	//void SetMesh(const int& index, Mesh* mesh);
	void SetMesh(Mesh* mesh);
	void SetShader(MapShader* shader);
	void Render(ID3D12GraphicsCommandList* commandList, PCamera* camera);
	void ReleaseUploadBuffers();
	// void SetMaterial(Material* material);
	//void CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

	//void SetGPUDescriptorHandle(const D3D12_GPU_DESCRIPTOR_HANDLE& gpuDescriptorHandle);
	//void SetGPUDescriptorHandlePtr(const unsigned long long& gpuDescriptorHandlePtr);
};