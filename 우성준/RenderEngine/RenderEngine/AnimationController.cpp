#include "framework.h"
#include "AnimationController.h"

AnimationController::AnimationController(FbxScene* fbxScene)
{
	FbxArray<FbxString*> fbxAnimationStackNames;
	fbxScene->FillAnimStackNameArray(fbxAnimationStackNames);

	animationStacks = fbxAnimationStackNames.Size();

	pAnimationStacks = new FbxAnimStack * [animationStacks];
	startTimes = new FbxTime[animationStacks];
	stopTimes = new FbxTime[animationStacks];
	currTimes = new FbxTime[animationStacks];

	for (int i = 0; i < animationStacks; i++)
	{
		FbxString* pfbxStackName = fbxAnimationStackNames[i];
		FbxAnimStack* pfbxAnimationStack = fbxScene->FindMember<FbxAnimStack>(pfbxStackName->Buffer());
		pAnimationStacks[i] = pfbxAnimationStack;

		FbxTakeInfo* pfbxTakeInfo = fbxScene->GetTakeInfo(*pfbxStackName);
		FbxTime fbxStartTime, fbxStopTime;
		if (pfbxTakeInfo)
		{
			fbxStartTime = pfbxTakeInfo->mLocalTimeSpan.GetStart();
			fbxStopTime = pfbxTakeInfo->mLocalTimeSpan.GetStop();
		}
		else
		{
			FbxTimeSpan fbxTimeLineTimeSpan;
			fbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(fbxTimeLineTimeSpan);
			fbxStartTime = fbxTimeLineTimeSpan.GetStart();
			fbxStopTime = fbxTimeLineTimeSpan.GetStop();
		}

		startTimes[i] = fbxStartTime;
		stopTimes[i] = fbxStopTime;
		currTimes[i] = FbxTime(0);
	}

	FbxArrayDelete(fbxAnimationStackNames);
}

void AnimationController::SetAnimationStack(FbxScene* fbxScene, int animationStack)
{
	this->animationStack = animationStack;
	fbxScene->SetCurrentAnimationStack(pAnimationStacks[animationStack]);
}

void AnimationController::AdvanceTime(float eTime)
{
	this->time += eTime;

	FbxTime fbxElapsedTime;
	fbxElapsedTime.SetSecondDouble(eTime);

	currTimes[animationStack] += fbxElapsedTime;
	if (currTimes[animationStack] > stopTimes[animationStack]) 
		currTimes[animationStack] = startTimes[animationStack];

}

FbxTime AnimationController::GetCurrentTime()
{
	return(currTimes[animationStack]);
}

void AnimationController::SetPosition(int animationStack, float position)
{

	currTimes[animationStack].SetSecondDouble(position);
}
