#include "stdafx.h"
#include "ModelInfo.h"
#include "SkinnedMesh.h"
#include "Object.h"

ModelInfo::~ModelInfo()
{
	SAFE_DELETEARR(m_ppSkinnedMeshes);
}

void ModelInfo::PrepareSkinning()
{
	int nSkinnedMesh = 0;
	m_ppSkinnedMeshes = new SkinnedMesh * [m_nSkinnedMeshes];
	m_pModelRootObject->FindAndSetSkinnedMesh(m_ppSkinnedMeshes, &nSkinnedMesh);

	for (int i = 0; i < m_nSkinnedMeshes; i++) m_ppSkinnedMeshes[i]->PrepareSkinning(m_pModelRootObject);
}