#pragma once
class AnimationController
{
public:
	
	AnimationController() = default;
	AnimationController(FbxScene* fbxScene);
	AnimationController(const AnimationController&) = delete;
	AnimationController& operator=(const AnimationController&) = delete;
	~AnimationController() = default;

private:

	float time = 0.f;
	int animationStacks = 0;
	FbxAnimStack** pAnimationStacks = nullptr;

	int animationStack = 0;

	FbxTime* startTimes = nullptr;
	FbxTime* stopTimes = nullptr;
	FbxTime* currTimes = nullptr;

public:

	void SetAnimationStack(FbxScene* fbxScene, int animationStack);
	
	void AdvanceTime(float eTime);
	
	FbxTime GetCurrentTime();

	void SetPosition(int animationStack, float position);

};

