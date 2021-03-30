#pragma once

class AnimationTrack;
class AnimationSets;
class SkinnedMesh;
class ModelInfo;
class Object;
class AnimationCallbackHandler;

class AnimationController final
{
public:

	AnimationController();
	AnimationController(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nAnimationTracks, ModelInfo* pModel);
	~AnimationController();

public:

	float time = 0.f;

	vector<AnimationTrack> animationTracks;
	
	int skinnedMeshCount = 0;
	vector<SkinnedMesh*> skinnedMeshes;
	AnimationSets* animationSets = nullptr;

	ID3D12Resource** cbSkinningBoneTransforms = nullptr;

	XMFLOAT4X4** cbMappedSkinningBoneTransforms = nullptr;

public:

	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	void SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet);
	void SetTrackEnable(int nAnimationTrack, bool bEnable);
	void SetTrackPosition(int nAnimationTrack, float fPosition);
	void SetTrackSpeed(int nAnimationTrack, float fSpeed);
	void SetTrackWeight(int nAnimationTrack, float fWeight);

	void SetCallbackKeys(int nAnimationSet, int nCallbackKeys);
	void SetCallbackKey(int nAnimationSet, int nKeyIndex, float fTime, void* pData);
	void SetAnimationCallbackHandler(int nAnimationSet, AnimationCallbackHandler* pCallbackHandler);

	void AdvanceTime(float fElapsedTime, Object* pRootGameObject);

};