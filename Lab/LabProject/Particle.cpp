#include "stdafx.h"
#include "Particle.h"
#include "ModelInfo.h"
#include "CrowdObject.h"
#include "AnimationController.h"
#include <random>

void Particle::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* rootSignatrue)
{
	uniform_real_distribution<> uid(-1.f, 1.f);
	default_random_engine dre;
	if (objectsRed.empty())
	{
		ModelInfo* cube = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, rootSignatrue, "Model/red.bin", nullptr);
		for (int i = 0; i < 150; ++i)
		{
			Object* temp = new CrowdObject(pd3dDevice, pd3dCommandList, rootSignatrue, cube, 1);
			temp->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
			//temp->SetScale(0.3f, .3f, .3f);
			temp->particleDir = XMFLOAT3(uid(dre),uid(dre), uid(dre));
			objectsRed.emplace_back(temp);
		}
	}
	if (objectsGreen.empty())
	{
		ModelInfo* cube = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, rootSignatrue, "Model/Green.bin", nullptr);
		for (int i = 0; i < 150; ++i)
		{
			Object* temp = new CrowdObject(pd3dDevice, pd3dCommandList, rootSignatrue, cube, 1);
			temp->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
			//temp->SetScale(0.3f, .3f, .3f);
			temp->particleDir = XMFLOAT3(uid(dre), uid(dre), uid(dre));
			objectsGreen.emplace_back(temp);
		}
	}
	if (objectsBlue.empty())
	{
		ModelInfo* cube = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, rootSignatrue, "Model/blue.bin", nullptr);
		for (int i = 0; i < 150; ++i)
		{
			Object* temp = new CrowdObject(pd3dDevice, pd3dCommandList, rootSignatrue, cube, 1);
			temp->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
			//temp->SetScale(0.3f, .3f, .3f);
			temp->particleDir = XMFLOAT3((float)uid(dre), (float)uid(dre), (float)uid(dre));
			objectsBlue.emplace_back(temp);
		}
	}
}

void Particle::PositionInit(XMFLOAT3 position, XMFLOAT3 direction, int particleColor)
{
	uniform_real_distribution<> uid(-1.f, 1.f);
	default_random_engine dre;
	ParticleON();
	objectColor = particleColor;
	switch (objectColor)
	{
	case REDCOLOR:
	{
		for (auto& i : objectsRed)
		{
			i->SetPosition(position);
			i->particleDir.x = (float)uid(dre);
			i->particleDir.y = (float)uid(dre);
			i->particleDir.z = direction.z;
		}
		break;
	}
	case GREENCOLOR:
	{
		for (auto& i : objectsGreen)
		{
			i->SetPosition(position);
			i->particleDir.x = (float)uid(dre);
			i->particleDir.y = (float)uid(dre);
			i->particleDir.z = direction.z;
		}
		break;
	}
	case BLUECOLOR:
	{
		for (auto& i : objectsBlue)
		{
			i->SetPosition(position);
			i->particleDir.x = (float)uid(dre);
			i->particleDir.y = (float)uid(dre);
			i->particleDir.z = direction.z;
		}
		break;
	}
	default:
		break;
	}

}

void Particle::ParticleON()
{
	lifeTime = 0.5f;
	switch (objectColor)
	{
	case REDCOLOR:
	{
		for (auto& i : objectsRed)
		{
			i->isActive = true;
		}
		break;
	}
	case GREENCOLOR:
	{
		for (auto& i : objectsGreen)
		{
			i->isActive = true;
		}
		break;
	}
	case BLUECOLOR:
	{
		for (auto& i : objectsBlue)
		{
			i->isActive = true;
		}
		break;
	}
	default:
		break;
	}
	

}

void Particle::ParticleOFF()
{
	switch (objectColor)
	{
	case REDCOLOR:
	{
		for (auto& i : objectsRed)
		{
			i->isActive = false;
		}
		break;
	}
	case GREENCOLOR:
	{
		for (auto& i : objectsGreen)
		{
			i->isActive = false;
		}
		break;
	}
	case BLUECOLOR:
	{
		for (auto& i : objectsBlue)
		{
			i->isActive = false;
		}
		break;
	}
	default:
		break;
	}
}

void Particle::Update(XMFLOAT3 position, float eTime)
{
	m_fElapsedTime = eTime;
	lifeTime -= m_fElapsedTime;
	switch (objectColor)
	{
	case REDCOLOR:
	{
		for (auto& i : objectsRed)
		{
			i->Move(i->particleDir);
			i->UpdateTransform(nullptr);
		}
		break;
	}
	case GREENCOLOR:
	{
		for (auto& i : objectsGreen)
		{
			i->Move(i->particleDir);
			i->UpdateTransform(nullptr);
		}
		break;
	}
	case BLUECOLOR:
	{
		for (auto& i : objectsBlue)
		{
			i->Move(i->particleDir);
			i->UpdateTransform(nullptr);
		}
		break;
	}
	default:
		break;
	}
	// «— πÊ«‚

	if (lifeTime < 0.f)
	{
		ParticleOFF();
	}
}

void Particle::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	switch (objectColor)
	{
	case REDCOLOR:
	{
		for (auto& object : objectsRed)
		{
			object->UpdateTransform(nullptr);
			object->Render(pd3dCommandList, pCamera);
		}
		break;
	}
	case GREENCOLOR:
	{
		for (auto& object : objectsGreen)
		{
			object->UpdateTransform(nullptr);
			object->Render(pd3dCommandList, pCamera);
		}
		break;
	}
	case BLUECOLOR:
	{
		for (auto& object : objectsBlue)
		{
			object->UpdateTransform(nullptr);
			object->Render(pd3dCommandList, pCamera);
		}
		break;
	}
	default:
		break;
	}

}
