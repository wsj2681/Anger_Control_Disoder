#pragma once



struct CALLBACKKEY;
class AnimationCallbackHandler;
class Object;

class CAnimationSet
{
public:
	CAnimationSet(float fLength, int nFramesPerSecond, int nKeyFrameTransforms, int nSkinningBones, char* pstrName);
	~CAnimationSet();

public:
	char							m_pstrAnimationSetName[64];

	float							m_fLength = 0.0f;
	int								m_nFramesPerSecond = 0; //m_fTicksPerSecond

	int								m_nKeyFrames = 0;
	float* m_pfKeyFrameTimes = nullptr;
	XMFLOAT4X4** m_ppxmf4x4KeyFrameTransforms = nullptr;

#ifdef _WITH_ANIMATION_SRT
	int								m_nKeyFrameScales = 0;
	float* m_pfKeyFrameScaleTimes = nullptr;
	XMFLOAT3** m_ppxmf3KeyFrameScales = nullptr;
	int								m_nKeyFrameRotations = 0;
	float* m_pfKeyFrameRotationTimes = nullptr;
	XMFLOAT4** m_ppxmf4KeyFrameRotations = nullptr;
	int								m_nKeyFrameTranslations = 0;
	float* m_pfKeyFrameTranslationTimes = nullptr;
	XMFLOAT3** m_ppxmf3KeyFrameTranslations = nullptr;
#endif

	float 							m_fPosition = 0.0f;
	int 							m_nType = ANIMATION_TYPE_ONCE; //Once, Loop, PingPong

	bool playOnce{ false };

	int 							m_nCallbackKeys = 0;
	CALLBACKKEY* m_pCallbackKeys = NULL;

	AnimationCallbackHandler* m_pAnimationCallbackHandler = nullptr;

public:
	void SetPosition(float fTrackPosition);

	XMFLOAT4X4 GetSRT(int nBone);

	void SetCallbackKeys(int nCallbackKeys);
	void SetCallbackKey(int nKeyIndex, float fTime, void* pData);
	void SetAnimationCallbackHandler(AnimationCallbackHandler* pCallbackHandler);

	void* GetCallbackData();
};

class AnimationSets
{
private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

public:
	AnimationSets(int nAnimationSets);
	~AnimationSets();

public:
	int								m_nAnimationSets = 0;
	CAnimationSet** m_pAnimationSets = nullptr;
	int m_nCurrSet = 0;

	int								m_nAnimatedBoneFrames = 0;
	Object** m_ppAnimatedBoneFrameCaches = nullptr; //[m_nAnimatedBoneFrames]

public:
	void SetCallbackKeys(int nAnimationSet, int nCallbackKeys);
	void SetCallbackKey(int nAnimationSet, int nKeyIndex, float fTime, void* pData);
	void SetAnimationCallbackHandler(int nAnimationSet, AnimationCallbackHandler* pCallbackHandler);
};