#include "stdafx.h"
#include "Particle.h"
#include "ModelInfo.h"
#include "BoxerObject.h"
#include "AnimationController.h"
#include <random>

void Particle::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* rootSignatrue)
{
	uniform_real_distribution<> uid(-1.0, 1.0);
	default_random_engine dre;
	if (objects.empty())
	{
		ModelInfo* cube = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, rootSignatrue, "Model/Sphere.bin", nullptr);
		for (int i = 0; i < 150; ++i)
		{
			Object* temp = new BoxerObject(pd3dDevice, pd3dCommandList, rootSignatrue, cube, 1);
			temp->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
			temp->SetScale(0.3f, .3f, .3f);
			temp->particleDir = XMFLOAT3(uid(dre),uid(dre), uid(dre));
			objects.emplace_back(temp);
		}
	}
}

void Particle::PositionInit(XMFLOAT3 position)
{
	ParticleON();
	for (auto& i : objects)
	{
		i->SetPosition(position);
	}
}

void Particle::ParticleON()
{
	lifeTime = 1.f;
	for (auto& i : objects)
	{
		i->isActive = true;
	}
}

void Particle::ParticleOFF()
{
	for (auto& i : objects)
	{
		i->isActive = false;
	}
}

void Particle::Update(XMFLOAT3 position, float eTime)
{
	m_fElapsedTime = eTime;
	lifeTime -= m_fElapsedTime;

	// ÇÑ ¹æÇâ
	for (auto& i : objects)
	{
		i->Move(i->particleDir);
		i->UpdateTransform(nullptr);
	}
	if (lifeTime < 0.f)
	{
		ParticleOFF();
	}
}

void Particle::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	for (auto& object : objects)
	{
		object->UpdateTransform(nullptr);
		object->Render(pd3dCommandList, pCamera);
	}
}
