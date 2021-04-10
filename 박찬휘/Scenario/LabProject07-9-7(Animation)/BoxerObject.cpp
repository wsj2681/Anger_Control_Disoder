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
	//cout << "BoxerObject::UpdateWayPoints" << endl;

	XMFLOAT3 pos = GetPosition();
	int curWayPoint = wayPoint.GetCurWayPoints();
	XMFLOAT3 direction = Vector3::Subtract(pos, wayPoint.GetWayPoints()[curWayPoint]);
	//float length = Vector3::Length(direction);
	//if (length < 100)	// obj_speed
	cout << direction.x << ", " << direction.y << ", " << direction.z << endl;
	if (false)
	{
		wayPoint.SetCurWayPoints(++curWayPoint);
		SetLook(wayPoint.GetWayPoints()[curWayPoint]);
	}
	else
	{
		MoveTo(wayPoint.GetWayPoints()[curWayPoint], 0.001f);
	}
	wayPoint.SetNowState(STATE_MOVE);
}