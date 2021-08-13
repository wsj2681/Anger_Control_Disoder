#include "stdafx.h"
#include "AnimationController.h"
#include "AnimationTrack.h"
#include "AnimationSet.h"
#include "ModelInfo.h"
#include "SkinnedMesh.h"
#include "Object.h"
#include "Scene.h"

extern Scene* gScene;

AnimationController::AnimationController(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nAnimationTracks, ModelInfo* pModel)
{
	m_nAnimationTracks = nAnimationTracks;
	m_pAnimationTracks = new AnimationTrack[nAnimationTracks];

	m_pAnimationSets = pModel->m_pAnimationSets;
	m_pAnimationSets->AddRef();

	m_nSkinnedMeshes = pModel->m_nSkinnedMeshes;
	m_ppSkinnedMeshes = new SkinnedMesh * [m_nSkinnedMeshes];
	for (int i = 0; i < m_nSkinnedMeshes; i++) m_ppSkinnedMeshes[i] = pModel->m_ppSkinnedMeshes[i];

	m_ppd3dcbSkinningBoneTransforms = new ID3D12Resource * [m_nSkinnedMeshes];
	m_ppcbxmf4x4MappedSkinningBoneTransforms = new XMFLOAT4X4 * [m_nSkinnedMeshes];

	UINT ncbElementBytes = (((sizeof(XMFLOAT4X4) * SKINNED_ANIMATION_BONES) + 255) & ~255); //256ÀÇ ¹è¼ö
	for (int i = 0; i < m_nSkinnedMeshes; i++)
	{
		m_ppd3dcbSkinningBoneTransforms[i] = ::CreateBufferResource(pd3dDevice, pd3dCommandList, nullptr, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, nullptr);
		m_ppd3dcbSkinningBoneTransforms[i]->Map(0, nullptr, (void**)&m_ppcbxmf4x4MappedSkinningBoneTransforms[i]);
	}
}

AnimationController::~AnimationController()
{
	SAFE_DELETEARR(m_pAnimationTracks);

	for (int i = 0; i < m_nSkinnedMeshes; i++)
	{
		m_ppd3dcbSkinningBoneTransforms[i]->Unmap(0, nullptr);
		SAFE_RELEASE(m_ppd3dcbSkinningBoneTransforms[i]);
	}
	SAFE_DELETEARR(m_ppd3dcbSkinningBoneTransforms);
	SAFE_DELETEARR(m_ppcbxmf4x4MappedSkinningBoneTransforms);

	SAFE_RELEASE(m_pAnimationSets);

	SAFE_DELETEARR(m_ppSkinnedMeshes);
}

void AnimationController::SetCallbackKeys(int nAnimationSet, int nCallbackKeys)
{
	if (m_pAnimationSets) m_pAnimationSets->SetCallbackKeys(nAnimationSet, nCallbackKeys);
}

void AnimationController::SetCallbackKey(int nAnimationSet, int nKeyIndex, float fKeyTime, void* pData)
{
	if (m_pAnimationSets) m_pAnimationSets->SetCallbackKey(nAnimationSet, nKeyIndex, fKeyTime, pData);
}

void AnimationController::SetAnimationCallbackHandler(int nAnimationSet, AnimationCallbackHandler* pCallbackHandler)
{
	if (m_pAnimationSets) m_pAnimationSets->SetAnimationCallbackHandler(nAnimationSet, pCallbackHandler);
}

void AnimationController::SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet, UINT nType, bool bAnimFixed)
{
	if (m_pAnimationTracks)
	{
		/*if (m_pAnimationSets)
		{
			if (!m_pAnimationSets->m_pAnimationSets[m_pAnimationTracks->m_nAnimationSet]->IsAnimate())

				switch (m_pAnimationSets->m_pAnimationSets[m_pAnimationTracks->m_nAnimationSet]->m_nType)
				{
				case ANIMATION_TYPE_LOOP:
				case ANIMATION_TYPE_PINGPONG:
					if (nType != ANIMATION_TYPE_ONCE)
						break;
				case ANIMATION_TYPE_ONCE:
					m_pAnimationTracks->m_fPosition = 0.f;
					break;
				default:
					break;
				}
		}*/
		if (bAnimFixed)
		{
			if (m_pAnimationSets)
			{
				switch (m_pAnimationSets->m_pAnimationSets[m_pAnimationTracks->m_nAnimationSet]->m_nType)
				{
				case ANIMATION_TYPE_LOOP:
				case ANIMATION_TYPE_PINGPONG:
					if (nType != ANIMATION_TYPE_ONCE)
						break;
				case ANIMATION_TYPE_ONCE:
					m_pAnimationTracks->m_fPosition = 0.f;
					break;
				default:
					break;
				}
			}
		}

		m_pAnimationTracks[nAnimationTrack].m_nAnimationSet = nAnimationSet;

		if (m_pAnimationSets)
			m_pAnimationSets->m_pAnimationSets[nAnimationSet]->m_nType = nType;
		if (gScene)
		{
			if (gScene->m_pPlayer)
				gScene->m_pPlayer->oldSpinePosition = gScene->m_pPlayer->bones["Spine"]->GetPosition();
		}
	}
}

void AnimationController::SetTrackEnable(int nAnimationTrack, bool bEnable)
{
	if (m_pAnimationTracks) m_pAnimationTracks[nAnimationTrack].SetEnable(bEnable);
}

void AnimationController::SetTrackPosition(int nAnimationTrack, float fPosition)
{
	if (m_pAnimationTracks) m_pAnimationTracks[nAnimationTrack].SetPosition(fPosition);
}

void AnimationController::SetTrackSpeed(int nAnimationTrack, float fSpeed)
{
	if (m_pAnimationTracks) m_pAnimationTracks[nAnimationTrack].SetSpeed(fSpeed);
}

void AnimationController::SetTrackWeight(int nAnimationTrack, float fWeight)
{
	if (m_pAnimationTracks) m_pAnimationTracks[nAnimationTrack].SetWeight(fWeight);
}

void AnimationController::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	for (int i = 0; i < m_nSkinnedMeshes; i++)
	{
		m_ppSkinnedMeshes[i]->m_pd3dcbSkinningBoneTransforms = m_ppd3dcbSkinningBoneTransforms[i];
		m_ppSkinnedMeshes[i]->m_pcbxmf4x4MappedSkinningBoneTransforms = m_ppcbxmf4x4MappedSkinningBoneTransforms[i];
	}
}

void AnimationController::AdvanceTime(float fTimeElapsed, Object* pRootGameObject)
{
	m_fTime += fTimeElapsed;
	if (m_pAnimationTracks)
	{
		for (int i = 0; i < m_nAnimationTracks; i++) m_pAnimationTracks[i].m_fPosition += (fTimeElapsed * m_pAnimationTracks[i].m_fSpeed);

		for (int j = 0; j < m_pAnimationSets->m_nAnimatedBoneFrames; j++)
		{
			XMFLOAT4X4 xmf4x4Transform = Matrix4x4::Zero();
			for (int k = 0; k < m_nAnimationTracks; k++)
			{
				if (m_pAnimationTracks[k].m_bEnable)
				{
					AnimationSet* pAnimationSet = m_pAnimationSets->m_pAnimationSets[m_pAnimationTracks[k].m_nAnimationSet];
					pAnimationSet->SetPosition(m_pAnimationTracks[k].m_fPosition);
					XMFLOAT4X4 xmf4x4TrackTransform = pAnimationSet->GetSRT(j);
					xmf4x4Transform = Matrix4x4::Add(xmf4x4Transform, Matrix4x4::Scale(xmf4x4TrackTransform, m_pAnimationTracks[k].m_fWeight));
				}
			}
			m_pAnimationSets->m_ppAnimatedBoneFrameCaches[j]->m_xmf4x4ToParent = xmf4x4Transform;
		}

		pRootGameObject->UpdateTransform(nullptr);
	}
}

bool AnimationController::IsAnimate()
{
	return m_pAnimationSets->m_pAnimationSets[m_pAnimationTracks[0].m_nAnimationSet]->IsAnimate();;
}

bool AnimationController::IsAnimate(int nAnimationSet)
{
	return m_pAnimationSets->m_pAnimationSets[nAnimationSet]->IsAnimate();
}

int AnimationController::GetNowTrackAnimationSet(int nAnimationTrack)
{
	if (m_pAnimationTracks)
	{
		return m_pAnimationTracks[nAnimationTrack].m_nAnimationSet;
	}

	return -1;
}