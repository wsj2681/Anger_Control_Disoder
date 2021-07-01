#pragma once

class PlaneObject;
class Camera;
class EffectManager
{
public:
	EffectManager(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	~EffectManager() = default;


private:
	vector<PlaneObject*> effectAnimation;

	int keyframe = 0;
	float elapsedTime = 0.f;
public:

	void Update(float deltaTime, XMFLOAT3 position);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera);

};
