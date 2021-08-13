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

		this->greenEffectAnimation.push_back(new PlaneObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pStr, isDDS));

	}

	for (int i = 0; i < 15; ++i)
	{
		if (i < 10)
		{
			sprintf(name, "Effect/Blueeffect/tile00%d.png", i);
		}
		else
		{
			sprintf(name, "Effect/Blueeffect/tile0%d.png", i);
		}

		wchar_t* pStr;
		int strSize = MultiByteToWideChar(CP_ACP, 0, name, -1, NULL, NULL);
		pStr = new WCHAR[strSize];
		MultiByteToWideChar(CP_ACP, 0, name, strlen(name) + 1, pStr, strSize);

		this->blueEffectAnimation.push_back(new PlaneObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pStr, isDDS));

	}

	for (int i = 0; i < 15; ++i)
	{
		if (i < 10)
		{
			sprintf(name, "Effect/circleeffect/tile00%d.png", i);
		}
		else
		{
			sprintf(name, "Effect/circleeffect/tile0%d.png", i);
		}

		wchar_t* pStr;
		int strSize = MultiByteToWideChar(CP_ACP, 0, name, -1, NULL, NULL);
		pStr = new WCHAR[strSize];
		MultiByteToWideChar(CP_ACP, 0, name, strlen(name) + 1, pStr, strSize);

		this->circleEffectAnimation.push_back(new PlaneObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pStr, isDDS));

	}

	for (int i = 0; i < 15; ++i)
	{
		if (i < 10)
		{
			sprintf(name, "Effect/redXEffect/tile00%d.png", i);
		}
		else
		{
			sprintf(name, "Effect/redXEffect/tile0%d.png", i);
		}

		wchar_t* pStr;
		int strSize = MultiByteToWideChar(CP_ACP, 0, name, -1, NULL, NULL);
		pStr = new WCHAR[strSize];
		MultiByteToWideChar(CP_ACP, 0, name, strlen(name) + 1, pStr, strSize);

		this->redXEffectAnimation.push_back(new PlaneObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pStr, isDDS));

	}

}

void EffectManager::EffectOn(XMFLOAT3 position, int effectType)
{
	this->effectType = effectType;
	switch (this->effectType)
	{
	case GREEN:
	{
		for (auto& i : greenEffectAnimation)
		{
			i->SetPosition(position);
		}
		break;
	}
	case BLUE:
	{
		for (auto& i : blueEffectAnimation)
		{
			i->SetPosition(position);
		}
		break;
	}
	case CIRCLE:
	{
		for (auto& i : circleEffectAnimation)
		{
			i->SetPosition(position);
		}
		break;
	}
	case REDX:
	{
		for (auto& i : redXEffectAnimation)
		{
			i->SetPosition(position);
		}
		break;
	}
	default:
		break;
	}

	keyframe = 0;
	isOn = true;
}

void EffectManager::EffectOff()
{
	isOn = false;
}

void EffectManager::Update(float deltaTime, XMFLOAT3 position)
{
	if ((elapsedTime += deltaTime) >= 0.01f)
	{
		switch (this->effectType)
		{
		case GREEN:
		{
			for (auto& i : greenEffectAnimation)
			{
				i->isActive = false;
			}
			greenEffectAnimation.data()[this->keyframe % greenEffectAnimation.size()]->isActive = true;
			keyframe++;
			elapsedTime = 0.f;

			if (this->keyframe % greenEffectAnimation.size() == greenEffectAnimation.size() - 1)
			{
				EffectOff();
			}
			break;
		}
		case BLUE:
		{
			for (auto& i : blueEffectAnimation)
			{
				i->isActive = false;
			}
			blueEffectAnimation.data()[this->keyframe % blueEffectAnimation.size()]->isActive = true;
			keyframe++;
			elapsedTime = 0.f;

			if (this->keyframe % blueEffectAnimation.size() == blueEffectAnimation.size() - 1)
			{
				EffectOff();
			}
			break;
		}
		case CIRCLE:
		{
			for (auto& i : circleEffectAnimation)
			{
				i->isActive = false;
			}
			circleEffectAnimation.data()[this->keyframe % circleEffectAnimation.size()]->isActive = true;
			keyframe++;
			elapsedTime = 0.f;

			if (this->keyframe % circleEffectAnimation.size() == circleEffectAnimation.size() - 1)
			{
				EffectOff();
			}
			break;
		}
		case REDX:
		{
			for (auto& i : redXEffectAnimation)
			{
				i->isActive = false;
			}
			redXEffectAnimation.data()[this->keyframe % redXEffectAnimation.size()]->isActive = true;
			keyframe++;
			elapsedTime = 0.f;

			if (this->keyframe % redXEffectAnimation.size() == redXEffectAnimation.size() - 1)
			{
				EffectOff();
			}
			break;
		}
		default:
			break;
		}

	}
}

void EffectManager::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	if (isOn)
	{
		switch (this->effectType)
		{
		case GREEN:
		{
			for (auto& i : greenEffectAnimation)
			{
				i->Render(pd3dCommandList, pCamera);
			}
			break;
		}
		case BLUE:
		{
			for (auto& i : blueEffectAnimation)
			{
				i->Render(pd3dCommandList, pCamera);
			}
			break;
		}
		case CIRCLE:
		{
			for (auto& i : circleEffectAnimation)
			{
				i->Render(pd3dCommandList, pCamera);
			}
			break;
		}
		case REDX:
		{
			for (auto& i : redXEffectAnimation)
			{
				i->Render(pd3dCommandList, pCamera);
			}
			break;
		}
		default:
			break;
		}

	}
}
