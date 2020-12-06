#pragma once
class Scene
{
public:
	Scene() = default;
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	~Scene() = default;
private:
	
public:
	void Render();
	void Animate();
	void ReleaseUploadBuffer();
	ID3D12RootSignature* CreateGraphicRootSignature(ID3D12Device* device);
};