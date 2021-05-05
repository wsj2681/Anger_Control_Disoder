#pragma once

#define ANIMATION_CALLBACK_EPSILON	0.015f

struct CALLBACKKEY
{
	float  							m_fTime = 0.0f;
	void* m_pCallbackData = nullptr;
};

class AnimationCallbackHandler
{
public:
	AnimationCallbackHandler() { }
	~AnimationCallbackHandler() { }

public:
	virtual void HandleCallback(void* pCallbackData);
};

class SoundCallbackHandler : public AnimationCallbackHandler
{
public:
	SoundCallbackHandler() { }
	~SoundCallbackHandler() { }

public:
	virtual void HandleCallback(void* pCallbackData);
};