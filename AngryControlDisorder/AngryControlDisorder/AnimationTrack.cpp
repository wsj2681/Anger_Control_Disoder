#include "AnimationTrack.h"

void AnimationTrack::SetAnimationSet(int nAnimationSet)
{
	this->animationSets = nAnimationSet;
}

void AnimationTrack::SetEnable(bool bEnable)
{
	this->enable = bEnable;
}

void AnimationTrack::SetSpeed(float fSpeed)
{
	this->speed = fSpeed;
}

void AnimationTrack::SetWeight(float fWeight)
{
	this->weight = fWeight;
}

void AnimationTrack::SetPosition(float fPosition)
{
	this->position = fPosition;
}
