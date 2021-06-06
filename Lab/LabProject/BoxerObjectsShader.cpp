#include "stdafx.h"
#include "BoxerObjectsShader.h"
#include "Object.h"

#include "ModelInfo.h"
#include "AnimationController.h"
#include "AnimationSet.h"
#include "AnimationTrack.h"
#include "BoxerObject.h"

BoxerObjectsShader::BoxerObjectsShader()
{
}

BoxerObjectsShader::~BoxerObjectsShader()
{
}

void BoxerObjectsShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, ModelInfo* pModel, void* pContext)
{
	int xObjects = 7, zObjects = 7, i = 0;

	m_nObjects = (xObjects * 2 + 1) * (zObjects * 2 + 1);

	m_ppObjects = new Object * [m_nObjects];

	float fxPitch = 7.0f * 2.5f;
	float fzPitch = 7.0f * 2.5f;

	ModelInfo* boxerModel = pModel;
	if (!boxerModel) boxerModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/ThaiBoxer.bin", nullptr);

	int nObjects = 0;
	for (int x = -xObjects; x <= xObjects; x++)
	{
		for (int z = -zObjects; z <= zObjects; z++)
		{
			m_ppObjects[nObjects] = new BoxerObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, boxerModel, 1);
			m_ppObjects[nObjects]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, (nObjects % 2));
			m_ppObjects[nObjects]->m_pSkinnedAnimationController->SetTrackSpeed(0, (nObjects % 2) ? 0.25f : 1.0f);
			m_ppObjects[nObjects]->m_pSkinnedAnimationController->SetTrackPosition(0, (nObjects % 3) ? 0.85f : 0.0f);
			XMFLOAT3 xmf3Position = XMFLOAT3(fxPitch * x + 390.0f, 0.0f, 730.0f + fzPitch * z);
			m_ppObjects[nObjects]->SetPosition(xmf3Position);
			m_ppObjects[nObjects++]->SetScale(2.0f, 2.0f, 2.0f);
		}
	}

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	if (!pModel && boxerModel) delete boxerModel;
}