#include "stdafx.h"
#include "CrowdObject.h"
#include "ModelInfo.h"
#include "AnimationController.h"

CrowdObject::CrowdObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, ModelInfo* pModel, int nAnimationTracks)
{
	ModelInfo* crowdModel = pModel;
	if (!crowdModel) crowdModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/BoxingComplete.bin", nullptr);

	SetChild(crowdModel->m_pModelRootObject, true);
	m_pSkinnedAnimationController = new AnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, crowdModel);
}

CrowdObject::~CrowdObject()
{
}