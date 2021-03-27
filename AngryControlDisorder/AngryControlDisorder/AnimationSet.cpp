#include "framework.h"
#include "AnimationSet.h"

AnimationSet::AnimationSet(float fLength, int nFramesPerSecond, int nKeyFrames, int nSkinningBones, char* pstrName)
{
	this->animationLength = fLength;
	this->framesPerSecond = nFramesPerSecond;
	this->keyFrames = nKeyFrames;

	strcpy_s(animationSetName, 64, pstrName);

	this->keyFrameTimes = new float[nKeyFrames];
	keyFrameTransforms = new XMFLOAT4X4*[nKeyFrames];

	for (int i = 0; i < nKeyFrames; ++i)
	{
		keyFrameTransforms[i] = new XMFLOAT4X4[nSkinningBones];
	}
}

AnimationSet::~AnimationSet()
{
}

void AnimationSet::SetPosition(const float& position)
{
	this->position = position;

	switch (type)
	{
	case ANIMATION_TYPE_LOOP:
	{
		break;
	}
	case ANIMATION_TYPE_ONCE:
	{
		break;
	}
	case ANIMATION_TYPE_PINGPONG:
	{
		break;
	}
	}
}

XMFLOAT4X4 AnimationSet::GetSRT(int bones)
{
	XMFLOAT4X4 xmf4x4Transform = Matrix4x4::Identity();
	for (int i = 0; i < (keyFrames - 1); i++)
	{
		if ((keyFrameTimes[i] <= position) && (position <= keyFrameTimes[i + 1]))
		{
			float t = (position - keyFrameTimes[i]) / (keyFrameTimes[i + 1] - keyFrameTimes[i]);
			xmf4x4Transform = Matrix4x4::Interpolate(keyFrameTransforms[i][bones], keyFrameTransforms[i + 1][bones], t);
			break;
		}
	}

	return xmf4x4Transform;
}

void AnimationSet::SetCallBackKeyCount(int callbackKey)
{
	this->callbackKeyCount = callbackKey;
	this->callbackKeys = new CALLBACKKEY[callbackKey];
}

void* AnimationSet::GetCallBackData()
{
	for (int i = 0; i < callbackKeyCount; i++)
	{
		if (::IsEqual(callbackKeys[i].time, position, ANIMATION_CALLBACK_EPSILON))
		{
			return callbackKeys[i].callbackData;
		}
	}
	return nullptr;
}

void AnimationSet::SetCallBackKey(int keyIndex, float time, void* data)
{
	callbackKeys[keyIndex].time = time;
	callbackKeys[keyIndex].callbackData = data;
}

void AnimationSet::SetAnimationCallBackHandler(AnimationCallbackHandler* callbackHander)
{
	animationCallbackHandler = callbackHander;
}
