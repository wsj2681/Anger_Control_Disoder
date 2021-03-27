#include "framework.h"
#include "AnimationSets.h"

AnimationSets::AnimationSets(int animationSets)
{
	animationSetCount = animationSets;
	this->animationSets = new AnimationSet * [animationSetCount];
}

AnimationSets::~AnimationSets()
{
}

void AnimationSets::AddRef()
{
	this->references++;
}

void AnimationSets::Release()
{
	if (--references <= 0)
	{
		delete this;
	}
}

void AnimationSets::SetCallbackkeys(int animationsets, int callbackKeys)
{
	animationSets[animationsets]->callbackKeyCount = callbackKeys;
	animationSets[animationsets]->callbackKeys = new CALLBACKKEY[callbackKeys];

}

void AnimationSets::SetCallbackKey(int animationsets, int keyIndex, float time, void* data)
{
	animationSets[animationsets]->callbackKeys[keyIndex].time = time;
	animationSets[animationsets]->callbackKeys[keyIndex].callbackData = data;

}

void AnimationSets::SetAnimationCallbackHandler(int animationSet, AnimationCallbackHandler* callbackHandler)
{
	animationSets[animationSet]->SetAnimationCallBackHandler(callbackHandler);
}
