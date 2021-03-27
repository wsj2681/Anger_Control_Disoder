#pragma once

#define ANIMATION_TYPE_ONCE			0
#define ANIMATION_TYPE_LOOP			1
#define ANIMATION_TYPE_PINGPONG		2

#define ANIMATION_CALLBACK_EPSILON	0.015f

struct CALLBACKKEY
{
	float time = 0.f;
	void* callbackData = nullptr;
};

class AnimationCallbackHandler
{
public:
	
	AnimationCallbackHandler() { }
	~AnimationCallbackHandler() { }

public:

	virtual void HandleCallback(void* pCallbackData) { }
};

class AnimationSet final
{
public:

	AnimationSet(float fLength, int nFramesPerSecond, int nKeyFrames, int nSkinningBones, char* pstrName);
	~AnimationSet();

public:

	char animationSetName[64];

	float animationLength = 0.f;
	int	framesPerSecond = 0; //m_fTicksPerSecond

	int	keyFrames = 0;
	float* keyFrameTimes = nullptr;
	XMFLOAT4X4** keyFrameTransforms = nullptr;

	float position = 0.f;
	int type = ANIMATION_TYPE_LOOP;

	int callbackKeyCount = 0;
	CALLBACKKEY* callbackKeys = nullptr;

	AnimationCallbackHandler* animationCallbackHandler = nullptr;

public:

	void SetPosition(const float& position);

	XMFLOAT4X4 GetSRT(int bones);

	void SetCallBackKeyCount(int callbackKey);
	
	void* GetCallBackData();
	void SetCallBackKey(int keyIndex, float time, void* data);

	void SetAnimationCallBackHandler(AnimationCallbackHandler* callbackHander);
};

