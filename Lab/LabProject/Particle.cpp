#include "stdafx.h"
#include "Particle.h"
#include "ModelInfo.h"
#include "BoxerObject.h"
#include "AnimationController.h"
#include <random>

void Particle::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* rootSignatrue)
{
	uniform_int_distribution<> uid(-500, 500);
	default_random_engine dre;
	if (objects.empty())
	{

		ModelInfo* cube = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, rootSignatrue, "Model/Cube.bin", nullptr);
		for (int i = 0; i < 100; ++i)
		{
			Object* temp = new BoxerObject(pd3dDevice, pd3dCommandList, rootSignatrue, cube, 1);
			temp->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
			temp->SetScale(1.5f, 1.5f, 1.5f);
			objects.emplace_back(temp);
		}
	}
}

void Particle::PositionInit(XMFLOAT3 position)
{
	for (auto& i : objects)
	{
		i->SetPosition(position);
	}
}

void Particle::ParticleON()
{
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

	for (auto& i : objects)
	{
		i->UpdateTransform(nullptr);
	}
	
	if (lifeTime < 0.f)
	{
		// ÆÄÆ¼Å¬ off
	}

}

void Particle::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	for (auto& object : objects)
	{
		object->Animate(m_fElapsedTime);
		if (object->m_pSkinnedAnimationController)
		{
			object->UpdateTransform(nullptr);
		}

		object->Render(pd3dCommandList, pCamera);
	}
}
