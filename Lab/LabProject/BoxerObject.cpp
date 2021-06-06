#include "stdafx.h"
#include "BoxerObject.h"
#include "ModelInfo.h"
#include "AnimationController.h"
#include "CollideCubeObject.h"
#include "CubeObject.h"
#include "AnimationSet.h"
#include "AnimationTrack.h"

BoxerObject::BoxerObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, ModelInfo* pModel, int nAnimationTracks)
{
	ModelInfo* boxerModel = pModel;
	if (!boxerModel) boxerModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/ThaiBoxer.bin", nullptr);


	SetChild(boxerModel->m_pModelRootObject, true);

	if (this->bones["head"] = FindFrame("Bip01_Head"))
	{
		this->boundBoxs["head"] = new CubeObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 1.f, 1.f, 1.f);
	}
	if (this->bones["rHand"] = FindFrame("Bip01_R_Hand"))
	{
		this->boundBoxs["rHand"] = new CubeObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 0.7f, 0.5f, 0.7f);
	}
	if (this->bones["lHand"] = FindFrame("Bip01_L_Hand"))
	{
		this->boundBoxs["lHand"] = new CubeObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 0.7f, 0.5f, 0.7f);
	}
	if (this->bones["lFoot"] = FindFrame("Bip01_L_Foot"))
	{
		this->boundBoxs["lFoot"] = new CubeObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 0.7f, 0.5f, 0.7f);
	}
	if (this->bones["rFoot"] = FindFrame("Bip01_R_Foot"))
	{
		this->boundBoxs["rFoot"] = new CubeObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 0.7f, 0.7f, 0.7f);
	}
	if (this->bones["Spine"] = FindFrame("Bip01_Spine1"))
	{
		this->boundBoxs["Spine"] = new CubeObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 2.0f, 1.f, 2.f);
	}

	//m_pSkinnedAnimationController = new AnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, boxerModel);
	m_pSkinnedAnimationController = new AnimationController(pd3dDevice, pd3dCommandList, 1, boxerModel);
	m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_COMBAT_MODE_A);
	for (int i = 0; i < m_pSkinnedAnimationController->m_pAnimationSets->m_nAnimationSets; ++i)
	{
		m_pSkinnedAnimationController->m_pAnimationSets->m_pAnimationSets[i]->isOtherPlayer = true;
		m_pSkinnedAnimationController->m_pAnimationSets->m_pAnimationSets[i]->m_nType = ANIMATION_TYPE_ONCE;
	}
}

BoxerObject::~BoxerObject()
{

}

void BoxerObject::UpdateWayPoints()
{
	int curWayPoint = wayPoint.GetCurWayPoints();
	if (curWayPoint < wayPoint.GetNWayPoints())
		MoveTo(wayPoint.GetWayPoints()[curWayPoint]);
	else
	{
		SetPosition(0.0f, 10.0f, -36.0f);
		SetLook(0.0f, 0.0f, 1.0f);
		m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_COMBAT_MODE_A);
	}
}