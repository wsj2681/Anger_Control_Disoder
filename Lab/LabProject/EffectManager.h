#pragma once

class PlaneObject;
class Camera;

enum EFFECT_TYPE
{
	GREEN,
	BLUE,
	CIRCLE
};

class EffectManager
{
public:
	EffectManager(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	~EffectManager() = default;


private:
	vector<PlaneObject*> greenEffectAnimation;
	vector<PlaneObject*> blueEffectAnimation;
	vector<PlaneObject*> circleEffectAnimation;


	int keyframe = 0;
	float elapsedTime = 0.f;

	bool isOn = false;

	int effectType = GREEN;
public:

	void EffectOn(XMFLOAT3 position, int effectType = 0);
	void EffectOff();
	void Update(float deltaTime, XMFLOAT3 position);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera);

};
