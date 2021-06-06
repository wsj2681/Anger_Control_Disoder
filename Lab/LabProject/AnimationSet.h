#pragma once

constexpr UINT ANIMATION_TYPE_ONCE = 0;
constexpr UINT ANIMATION_TYPE_LOOP = 1;
constexpr UINT ANIMATION_TYPE_PINGPONG = 2;

struct CALLBACKKEY;
class AnimationCallbackHandler;
class Object;

class AnimationSet final
{
public:
	AnimationSet() = default;
	AnimationSet(float fLength, int nFramesPerSecond, int nKeyFrameTransforms, int nSkinningBones, char* pstrName);
	AnimationSet(const AnimationSet&) = delete;
	AnimationSet& operator=(const AnimationSet&) = delete;
	AnimationSet(AnimationSet&&) = delete;
	AnimationSet& operator=(AnimationSet&&) = delete;
	~AnimationSet();
private:
	char							m_pstrAnimationSetName[64];

	float							m_fLength = 0.0f;
	int								m_nFramesPerSecond = 0; //m_fTicksPerSecond

	int								m_nKeyFrames = 0;
	
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

public :
	int 							m_nType = ANIMATION_TYPE_LOOP; //Once, Loop, PingPong

	AnimationCallbackHandler* m_pAnimationCallbackHandler = nullptr;
public:
	float 							m_fPosition = 0.0f;
	int 							m_nCallbackKeys = 0;
	CALLBACKKEY* m_pCallbackKeys = nullptr;

	float* m_pfKeyFrameTimes = nullptr;
	XMFLOAT4X4** m_ppxmf4x4KeyFrameTransforms = nullptr;

	bool isPlayer = false;
	bool isOtherPlayer = false;
private:
	void SetCallbackKeys(int nCallbackKeys);
	void SetCallbackKey(int nKeyIndex, float fTime, void* pData);

	void* GetCallbackData();
public:
	void SetPosition(float fTrackPosition);

	XMFLOAT4X4 GetSRT(int nBone);

	void SetAnimationCallbackHandler(AnimationCallbackHandler* pCallbackHandler);
	bool IsAnimate();
};

class AnimationSets final
{
public:
	AnimationSets() = default;
	AnimationSets(int nAnimationSets);
	AnimationSets(const AnimationSets&) = delete;
	AnimationSets& operator=(const AnimationSets&) = delete;
	AnimationSets(AnimationSets&&) = delete;
	AnimationSets& operator=(AnimationSets&&) = delete;
	~AnimationSets();
private:
	int								m_nReferences = 0;
public:
	int	m_nAnimationSets = 0;
	AnimationSet** m_pAnimationSets = nullptr;

	int	m_nAnimatedBoneFrames = 0;
	Object** m_ppAnimatedBoneFrameCaches = nullptr; //[m_nAnimatedBoneFrames]
public:
	void AddRef();
	void Release();

	void SetCallbackKeys(int nAnimationSet, int nCallbackKeys);
	void SetCallbackKey(int nAnimationSet, int nKeyIndex, float fTime, void* pData);
	void SetAnimationCallbackHandler(int nAnimationSet, AnimationCallbackHandler* pCallbackHandler);
};