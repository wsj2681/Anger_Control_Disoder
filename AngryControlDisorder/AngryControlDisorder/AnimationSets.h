#pragma once

#include "AnimationSet.h"
#include "Object.h"

class AnimationSets final
{
public:

	AnimationSets() = default;
	AnimationSets(int animationSets);
	~AnimationSets();

private:

	int references = 0;

public:

	int animationSetCount = 0;
	AnimationSet** animationSets = nullptr;

	int animatedBoneFrameCount = 0;
	Object** animatedBoneFrameCaches = nullptr;

public:

	void AddRef();
	void Release();

	void SetCallbackkeys(int animationsets, int callbackKeys);
	void SetCallbackKey(int animationsets, int keyIndex, float time, void* data);
	void SetAnimationCallbackHandler(int animationSet, AnimationCallbackHandler* callbackHandler);

};

