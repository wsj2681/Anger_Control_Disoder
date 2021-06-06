#include "stdafx.h"
#include "BoxerObject.h"
#include "ModelInfo.h"
#include "AnimationController.h"
#include "CollideCubeObject.h"
#include "CubeObject.h"

BoxerObject::BoxerObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, ModelInfo* pModel, int nAnimationTracks)
{
	ModelInfo* boxerModel = pModel;
	if (!boxerModel) boxerModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/ThaiBoxerD.bin", nullptr);


	SetChild(boxerModel->m_pModelRootObject, true);
	m_pSkinnedAnimationController = new AnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, boxerModel);

	if (this->bones["Head"] = FindFrame("Bip01_Head"))
	{
		this->boundBoxs["Head"] = new CubeObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 0.3f, 0.3f, 0.3f);
	}
	if (this->bones["rHand"] = FindFrame("Bip01_R_Hand"))
	{
		this->boundBoxs["rHand"] = new CubeObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 0.21f, 0.15f, 0.21f);
	}
	if (this->bones["lHand"] = FindFrame("Bip01_L_Hand"))
	{
		this->boundBoxs["lHand"] = new CubeObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 0.21f, 0.15f, 0.21f);
	}
	if (this->bones["lFoot"] = FindFrame("Bip01_L_Foot"))
	{
		this->boundBoxs["lFoot"] = new CubeObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 0.21f, 0.15f, 0.21f);
	}
	if (this->bones["rFoot"] = FindFrame("Bip01_R_Foot"))
	{
		this->boundBoxs["rFoot"] = new CubeObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 0.21f, 0.15f, 0.21f);
	}
	if (this->bones["Spine"] = FindFrame("Bip01_Spine1"))
	{
		this->boundBoxs["Spine"] = new CubeObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 0.6f, 0.3f, 0.3f);
		//this->boundBoxs["SpineLeft"] = new CubeObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 0.6f, 0.3f, 0.3f, "SpineLeft");
		//this->boundBoxs["SpineRight"] = new CubeObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 0.6f, 0.3f, 0.3f, "SpineRight");
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