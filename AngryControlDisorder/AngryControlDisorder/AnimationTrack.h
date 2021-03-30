#pragma once
class AnimationTrack
{
public:
	AnimationTrack() = default;
	~AnimationTrack() = default;

public:
	bool 							enable = true;
	float 							speed = 1.0f;
	float 							position = 0.0f;
	float 							weight = 1.0f;

	int 							animationSets = 0;

public:
	void SetAnimationSet(int nAnimationSet);

	void SetEnable(bool bEnable);
	void SetSpeed(float fSpeed);
	void SetWeight(float fWeight);
	void SetPosition(float fPosition);
};