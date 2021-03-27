#include "AnimationTrack.h"

void CAnimationTrack::SetAnimationSet(int nAnimationSet)
{
	this->animationSets = nAnimationSet;
}

void CAnimationTrack::SetEnable(bool bEnable)
{
	this->enable = bEnable;
}

void CAnimationTrack::SetSpeed(float fSpeed)
{
	this->speed = fSpeed;
}

void CAnimationTrack::SetWeight(float fWeight)
{
	this->weight = fWeight;
}

void CAnimationTrack::SetPosition(float fPosition)
{
	this->position = fPosition;
}
