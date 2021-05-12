#pragma once
#include "Object.h"


class Particle
{
public:
	Particle() = default;
	Particle(const Particle&) = delete;
	Particle& operator=(const Particle&) = delete;
	~Particle() = default;

private:

	XMFLOAT3 position{ 0.f, 0.f, 0.f };
	XMFLOAT3 dir{ 0.f, 0.f, 0.f };
	XMFLOAT3 rotateAxis{ 0.f, 0.f, 0.f };

	float lifeTime = 3.f;
	float m_fElapsedTime = 0.f;
	vector<Object*> objects;

public:

	void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* rootSignatrue);
	void PositionInit(XMFLOAT3 position);
	void ParticleON();
	void ParticleOFF();
	void Destroy();
	void Update(XMFLOAT3 position, float eTime);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera);
};

