#include "stdafx.h"
#include "BoxerObject.h"
#include "ModelInfo.h"
#include "AnimationController.h"

BoxerObject::BoxerObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, ModelInfo* pModel, int nAnimationTracks)
{
	ModelInfo* boxerModel = pModel;
	if (!boxerModel) boxerModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/ThaiBoxer.bin", nullptr);

	SetChild(boxerModel->m_pModelRootObject, true);
	m_pSkinnedAnimationController = new AnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, boxerModel);
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
		SetPosition(-0.579981f, 10.0f, 24.9725f);
		SetLook(0.0f, 0.0f, -1.0f);
	}
}