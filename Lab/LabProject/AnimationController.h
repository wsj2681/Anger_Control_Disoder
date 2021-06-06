#pragma once

class ModelInfo;
class AnimationTrack;
class AnimationSets;
class SkinnedMesh;
class AnimationCallbackHandler;
class Object;

class AnimationController
{
public:
	AnimationController() = default;
	AnimationController(const AnimationController&) = delete;
	AnimationController& operator=(const AnimationController&) = delete;
	AnimationController(AnimationController&&) = delete;
	AnimationController& operator=(AnimationController&&) = delete;
	AnimationController(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nAnimationTracks, ModelInfo* pModel);
	~AnimationController();

private:
	float m_fTime = 0.0f;
	int m_nAnimationTracks = 0;

	int m_nSkinnedMeshes = 0;
	SkinnedMesh** m_ppSkinnedMeshes = nullptr; //[SkinnedMeshes], Skinned Mesh Cache

	ID3D12Resource** m_ppd3dcbSkinningBoneTransforms = nullptr; //[SkinnedMeshes]
	XMFLOAT4X4** m_ppcbxmf4x4MappedSkinningBoneTransforms = nullptr; //[SkinnedMeshes]
public:
	AnimationSets* m_pAnimationSets = nullptr;
	AnimationTrack* m_pAnimationTracks = nullptr;
private:
	void SetTrackEnable(int nAnimationTrack, bool bEnable);
	void SetTrackWeight(int nAnimationTrack, float fWeight);

	void SetCallbackKeys(int nAnimationSet, int nCallbackKeys);
	void SetCallbackKey(int nAnimationSet, int nKeyIndex, float fTime, void* pData);

	bool IsAnimate();
	bool IsAnimate(int nAnimationSet);
public:
	void SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet, UINT nType = 0x00, bool bAnimFixed = false);
	void SetTrackPosition(int nAnimationTrack, float fPosition);
	void SetTrackSpeed(int nAnimationTrack, float fSpeed);
	void SetAnimationCallbackHandler(int nAnimationSet, AnimationCallbackHandler* pCallbackHandler);

	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	void AdvanceTime(float fElapsedTime, Object* pRootGameObject);
	int GetNowTrackAnimationSet(int nAnimationTrack);
};