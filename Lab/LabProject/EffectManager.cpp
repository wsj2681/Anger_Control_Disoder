#include "stdafx.h"
#include "EffectManager.h"
#include "Object.h"
#include "Camera.h"
#include "PlaneObject.h"

EffectManager::EffectManager(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	char name[50];
	bool isDDS = false;
	for (int i = 0; i < 10; ++i)
	{
		if (i < 10)
		{
			sprintf(name, "Effect/greeneffect/tile00%d.png", i);
		}
		else
		{
			sprintf(name, "Effect/greeneffect/tile0%d.png", i);
		}

		wchar_t* pStr;
		int strSize = MultiByteToWideChar(CP_ACP, 0, name, -1, NULL, NULL);
		pStr = new WCHAR[strSize];
		MultiByteToWideChar(CP_ACP, 0, name, strlen(name) + 1, pStr, strSize);

		this->effectAnimation.push_back(new PlaneObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pStr, isDDS));

	}
}

void EffectManager::Update(float deltaTime, XMFLOAT3 position)
{
	if ((elapsedTime += deltaTime) >= 0.05f)
	{
		for (auto& i : effectAnimation)
		{
			i->SetPosition(position);
			i->isActive = false;
		}
		effectAnimation.data()[this->keyframe % effectAnimation.size()]->isActive = true;
		keyframe++;
		elapsedTime = 0.f;
	}
}

void EffectManager::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	for (auto& i : effectAnimation)
	{
		i->Render(pd3dCommandList, pCamera);
	}
}
