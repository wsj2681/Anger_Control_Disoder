#include "framework.h"
#include "AnimationController.h"
#include "AnimationSets.h"
#include "AnimationTrack.h"
#include "SkinnedMesh.h"
#include "ModelInfo.h"

AnimationController::AnimationController()
{
}

AnimationController::AnimationController(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nAnimationTracks, ModelInfo* pModel)
{
	animationTracks.resize(sizeof(AnimationTrack) * nAnimationTracks);
	
	animationSets = pModel->animationSets;
	animationSets->AddRef();
	this->skinnedMeshCount = pModel->skinnedMeshCount;
	skinnedMeshes.resize(sizeof(SkinnedMesh) * skinnedMeshCount);

	for (int i = 0; i < pModel->skinnedMeshCount; ++i)
	{
		skinnedMeshes.data()[i] = pModel->skinnedMeshs[i];
	}
	cbSkinningBoneTransforms = new ID3D12Resource * [skinnedMeshCount];
	cbMappedSkinningBoneTransforms = new XMFLOAT4X4 * [skinnedMeshCount];

	UINT ncbElementBytes = (((sizeof(XMFLOAT4X4) * SKINNED_ANIMATION_BONES) + 255) & ~255); //256ÀÇ ¹è¼ö
	for (int i = 0; i < skinnedMeshCount; i++)
	{
		cbSkinningBoneTransforms[i] = ::CreateBufferResource(pd3dDevice, pd3dCommandList, nullptr, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, nullptr);
		cbSkinningBoneTransforms[i]->Map(0, nullptr, (void**)&cbMappedSkinningBoneTransforms[i]);
	}
}

AnimationController::~AnimationController()
{
	animationTracks.clear();

	for (int i = 0; i < skinnedMeshCount; ++i)
	{
		cbSkinningBoneTransforms[i]->Unmap(0, NULL);
		cbSkinningBoneTransforms[i]->Release();
	}

	DELETE_ARRAY(cbSkinningBoneTransforms);
	DELETE_ARRAY(cbMappedSkinningBoneTransforms);

	SAFE_RELEASE(animationSets);
	skinnedMeshes.clear();

}

void AnimationController::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	for (int i = 0; i < (int)skinnedMeshes.size(); i++)
	{
		skinnedMeshes.data()[i]->cbSkinningBoneTransforms = cbSkinningBoneTransforms[i];
		skinnedMeshes.data()[i]->cbMappedSkinningBoneTransforms = cbMappedSkinningBoneTransforms[i];
	}
}

void AnimationController::SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet)
{
	if (animationTracks.size() > 0)
	{
		this->time = 0;
		animationTracks.data()[nAnimationTrack].SetAnimationSet(nAnimationSet);
	}
}

void AnimationController::SetTrackEnable(int nAnimationTrack, bool bEnable)
{
	if (animationSets) animationTracks.data()[nAnimationTrack].SetEnable(bEnable);
}

void AnimationController::SetTrackPosition(int nAnimationTrack, float fPosition)
{
	if (animationSets) animationTracks.data()[nAnimationTrack].SetPosition(fPosition);
}

void AnimationController::SetTrackSpeed(int nAnimationTrack, float fSpeed)
{
	if (animationSets) animationTracks.data()[nAnimationTrack].SetSpeed(fSpeed);
}

void AnimationController::SetTrackWeight(int nAnimationTrack, float fWeight)
{
	if (animationSets) animationTracks.data()[nAnimationTrack].SetWeight(fWeight);
}

void AnimationController::SetCallbackKeys(int nAnimationSet, int nCallbackKeys)
{
	if (animationSets) animationSets->SetCallbackkeys(nAnimationSet, nCallbackKeys);
}

void AnimationController::SetCallbackKey(int nAnimationSet, int nKeyIndex, float fTime, void* pData)
{
	if (animationSets) animationSets->SetCallbackKey(nAnimationSet, nKeyIndex, fTime, pData);
}

void AnimationController::SetAnimationCallbackHandler(int nAnimationSet, AnimationCallbackHandler* pCallbackHandler)
{
	if (animationSets) animationSets->SetAnimationCallbackHandler(nAnimationSet, pCallbackHandler);
}

void AnimationController::AdvanceTime(float fElapsedTime, Object* pRootGameObject)
{
	this->time += fElapsedTime;
	if (animationTracks.size() > 0)
	{
		for (auto& animationTrack : animationTracks)
		{
			animationTrack.position += (fElapsedTime * animationTrack.speed);
		}


		for (int j = 0; j < animationSets->animatedBoneFrameCount; j++)
		{
			XMFLOAT4X4 xmf4x4Transform = Matrix4x4::Zero();
			for (int k = 0; k < (int)animationTracks.size(); k++)
			{
				if (animationTracks.data()[k].enable)
				{
					AnimationSet* pAnimationSet = animationSets->animationSets[animationTracks.data()[k].animationSets];
					pAnimationSet->SetPosition(animationTracks.data()[k].position);
					XMFLOAT4X4 xmf4x4TrackTransform = pAnimationSet->GetSRT(j);
					xmf4x4Transform = Matrix4x4::Add(xmf4x4Transform, Matrix4x4::Scale(xmf4x4TrackTransform, animationTracks.data()[k].weight));
				}
			}
			animationSets->animatedBoneFrameCaches[j]->SetToParent(xmf4x4Transform);
		}

		pRootGameObject->UpdateTransform(NULL);
	}
}
