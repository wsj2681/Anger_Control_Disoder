#include "stdafx.h"
#include "BoxerObject.h"
#include "ModelInfo.h"
#include "AnimationController.h"

BoxerObject::BoxerObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, ModelInfo* pModel, int nAnimationTracks)
{
	ModelInfo* boxerModel = pModel;
	if (!boxerModel) boxerModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/ThaiBoxerD.bin", nullptr);


	SetChild(boxerModel->m_pModelRootObject, true);
	m_pSkinnedAnimationController = new AnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, boxerModel);

	this->head = FindFrame("Bip01_Head");
	if (this->head)
		this->head->objectCollision = new BoundingOrientedBox(this->head->GetPosition(), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	this->rHand = FindFrame("Bip01_R_Hand");
	if (this->rHand)
		this->rHand->objectCollision = new BoundingOrientedBox(this->rHand->GetPosition(), XMFLOAT3(0.7f, 0.5f, 0.7f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	this->lHand = FindFrame("Bip01_L_Hand");
	if (this->lHand)
		this->lHand->objectCollision = new BoundingOrientedBox(this->lHand->GetPosition(), XMFLOAT3(0.7f, 0.5f, 0.7f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	this->rFoot = FindFrame("Bip01_L_Foot"); //
	this->lFoot = FindFrame("Bip01_R_Foot"); //
	this->spine = FindFrame("Bip01_Spine1");
	if (this->spine)
		this->spine->objectCollision = new BoundingOrientedBox(this->spine->GetPosition(), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
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