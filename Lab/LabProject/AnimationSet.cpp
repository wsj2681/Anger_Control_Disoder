#include "stdafx.h"
#include "AnimationSet.h"
#include "CallBack.h"
#include "Object.h"
#include "Player.h"
#include "Scene.h"
#include "AnimationController.h"
#include "AnimationTrack.h"

extern Scene* gScene;

AnimationSet::AnimationSet(float fLength, int nFramesPerSecond, int nKeyFrames, int nAnimatedBones, char* pstrName)
{
	m_fLength = fLength;
	m_nFramesPerSecond = nFramesPerSecond;
	m_nKeyFrames = nKeyFrames;
	m_nKeyFrameRotations = nKeyFrames;
	m_nKeyFrameScales = nKeyFrames;
	m_nKeyFrameTranslations = nKeyFrames;

	strcpy_s(m_pstrAnimationSetName, 64, pstrName);

	m_pfKeyFrameTimes = new float[nKeyFrames];
	m_ppxmf4x4KeyFrameTransforms = new XMFLOAT4X4 * [nKeyFrames];

	m_pfKeyFrameScaleTimes = new float[nKeyFrames];
	m_ppxmf3KeyFrameScales = new XMFLOAT3 * [nKeyFrames];

	m_pfKeyFrameTranslationTimes = new float[nKeyFrames];
	m_ppxmf3KeyFrameTranslations = new XMFLOAT3 * [nKeyFrames];

	m_pfKeyFrameRotationTimes = new float[nKeyFrames];
	m_ppxmf4KeyFrameRotations = new XMFLOAT4 * [nKeyFrames];

	for (int i = 0; i < nKeyFrames; i++) m_ppxmf4x4KeyFrameTransforms[i] = new XMFLOAT4X4[nAnimatedBones];
	for (int i = 0; i < nKeyFrames; i++) m_ppxmf3KeyFrameScales[i] = new XMFLOAT3[nAnimatedBones];
	for (int i = 0; i < nKeyFrames; i++) m_ppxmf3KeyFrameTranslations[i] = new XMFLOAT3[nAnimatedBones];
	for (int i = 0; i < nKeyFrames; i++) m_ppxmf4KeyFrameRotations[i] = new XMFLOAT4[nAnimatedBones];
}

AnimationSet::~AnimationSet()
{
	if (m_pfKeyFrameTimes) delete[] m_pfKeyFrameTimes;
	for (int j = 0; j < m_nKeyFrames; j++) if (m_ppxmf4x4KeyFrameTransforms[j]) delete[] m_ppxmf4x4KeyFrameTransforms[j];
	for (int j = 0; j < m_nKeyFrames; j++) if (m_ppxmf3KeyFrameScales[j]) delete[] m_ppxmf3KeyFrameScales[j];
	for (int j = 0; j < m_nKeyFrames; j++) if (m_ppxmf3KeyFrameTranslations[j]) delete[] m_ppxmf3KeyFrameTranslations[j];
	for (int j = 0; j < m_nKeyFrames; j++) if (m_ppxmf4KeyFrameRotations[j]) delete[] m_ppxmf4KeyFrameRotations[j];

	if (m_ppxmf4x4KeyFrameTransforms) delete[] m_ppxmf4x4KeyFrameTransforms;
	if (m_ppxmf3KeyFrameScales) delete[] m_ppxmf3KeyFrameScales;
	if (m_ppxmf3KeyFrameTranslations) delete[] m_ppxmf3KeyFrameTranslations;
	if (m_ppxmf4KeyFrameRotations) delete[] m_ppxmf4KeyFrameRotations;

	SAFE_DELETEARR(m_pCallbackKeys);
	SAFE_DELETE(m_pAnimationCallbackHandler);
}

void* AnimationSet::GetCallbackData()
{
	for (int i = 0; i < m_nCallbackKeys; i++)
	{
		if (::IsEqual(m_pCallbackKeys[i].m_fTime, m_fPosition, ANIMATION_CALLBACK_EPSILON)) 
			return(m_pCallbackKeys[i].m_pCallbackData);
	}
	return nullptr;
}

bool AnimationSet::IsAnimate()
{
	return (m_pfKeyFrameTimes[(m_nKeyFrames - 1)]) <= m_fPosition + 0.04f /* && (m_fPosition <= m_pfKeyFrameTimes[i + 1])*/;
}

void AnimationSet::SetPosition(float fTrackPosition)
{
	m_fPosition = fTrackPosition;

	switch (m_nType)
	{
	case ANIMATION_TYPE_LOOP:
	{
		m_fPosition = fmod(fTrackPosition, m_pfKeyFrameTimes[m_nKeyFrames - 1]);
		break;
	}
	case ANIMATION_TYPE_ONCE:
	{
		m_fPosition = fTrackPosition - int(fTrackPosition / m_pfKeyFrameTimes[m_nKeyFrames - 1]) * m_pfKeyFrameTimes[m_nKeyFrames - 1];

		if (IsAnimate())
		{
			if (isPlayer) // 애니메이션이 끝났을 때
			{
				if ((gScene->m_pPlayer->isAlive) && (gScene->m_pPlayer->nowState != IDLE))
				{
					gScene->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_COMBAT_MODE_A);
					gScene->m_pPlayer->nowState = IDLE;
				}
				gScene->m_pPlayer->m_pSkinnedAnimationController->m_pAnimationTracks[0].m_fPosition = 0.0f;
				m_fPosition = 0.0f;
			}
			else if (isOtherPlayer)
			{
				if ((gScene->hierarchicalGameObjects.data()[1]->isAlive) && (gScene->hierarchicalGameObjects.data()[1]->nowState != IDLE))
				{
					gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_COMBAT_MODE_A);
					gScene->hierarchicalGameObjects.data()[1]->nowState = IDLE;
				}
				gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->m_pAnimationTracks[0].m_fPosition = 0.0f;
				m_fPosition = 0.0f;
			}
			break;
		}
		else// 애니메이션중
		{

			if (isPlayer)
			{
				// 아무 입력도 없었을 때
				if (gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) != ANIMATION_COMBAT_MODE_A)
				{
					// 움직이는 상태가 아닐때
					if (gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) != ANIMATION_MOVE_FORWARD &&
						gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) != ANIMATION_MOVE_BACKWARD &&
						gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) != ANIMATION_MOVE_LEFT &&
						gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) != ANIMATION_MOVE_RIGHT)
					{
						// 가드 동작을 취하면
						if (gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_BODY ||
							gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_LEFT_HEAD ||
							gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_RIGHT_HEAD ||
							gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_LOW_LEFT ||
							gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_LOW_RIGHT ||
							gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_SIDE_LEFT ||
							gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_SIDE_RIGHT ||
							gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_HOOK_LEFT ||
							gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_HOOK_RIGHT)
						{
							if (gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_HOOK_LEFT ||
								gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_HOOK_RIGHT)
							{
								gScene->m_pPlayer->nowState = HIGH_GUARD;
							}
							else if (gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_SIDE_LEFT ||
								gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_SIDE_RIGHT)
							{
								gScene->m_pPlayer->nowState = MIDDLE_GUARD;
							}
							else if (gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_LOW_LEFT ||
								gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_LOW_RIGHT)
							{
								gScene->m_pPlayer->nowState = LOW_GUARD;
							}
						}
						// 맞는 중이다 라는 애니메이션
						else if (gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_HIT_TORSO_STRIGHT_B ||
							gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_HIT_HEAD_STRIGHT_B)
						{
							gScene->m_pPlayer->nowState = HIT;
						}
						else if(gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_COME_HERE_BRUCE_LI ||
							gScene->m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_COME_HERE_1HAND) // 가드가 아니고 맞는상황의 애니메이션이 아니라면
						{
							gScene->m_pPlayer->nowState = OTHER;
						}
						else
						{
							gScene->m_pPlayer->nowState = ATTACK;
						}
					}
					else
					{
						gScene->m_pPlayer->nowState = MOVE;
					}
				}
			}
			else if (isOtherPlayer)
			{
				if (gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) != ANIMATION_COMBAT_MODE_A)
				{
					// 움직이는 상태가 아닐때
					if (gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) != ANIMATION_MOVE_FORWARD &&
						gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) != ANIMATION_MOVE_BACKWARD &&
						gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) != ANIMATION_MOVE_LEFT &&
						gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) != ANIMATION_MOVE_RIGHT)
					{
						// 가드 동작을 취하면
						if (gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_BODY ||
							gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_LEFT_HEAD ||
							gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_RIGHT_HEAD ||
							gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_LOW_LEFT ||
							gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_LOW_RIGHT ||
							gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_SIDE_LEFT ||
							gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_SIDE_RIGHT ||
							gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_HOOK_LEFT ||
							gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_HOOK_RIGHT)
						{
							if (gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_HOOK_LEFT ||
								gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_HOOK_RIGHT)
							{
								gScene->hierarchicalGameObjects.data()[1]->nowState = HIGH_GUARD;
							}
							else if (gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_SIDE_LEFT ||
								gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_SIDE_RIGHT)
							{
								gScene->hierarchicalGameObjects.data()[1]->nowState = MIDDLE_GUARD;
							}
							else if (gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_LOW_LEFT ||
								gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_GUARD_LOW_RIGHT)
							{
								gScene->hierarchicalGameObjects.data()[1]->nowState = LOW_GUARD;
							}
						}
						// 맞는 중이다 라는 애니메이션
						else if (gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_HIT_TORSO_STRIGHT_B ||
							gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_HIT_HEAD_STRIGHT_B)
						{
							gScene->hierarchicalGameObjects.data()[1]->nowState = HIT;
						}
						else if (gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_COME_HERE_BRUCE_LI ||
							gScene->hierarchicalGameObjects.data()[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) == ANIMATION_COME_HERE_1HAND) // 가드가 아니고 맞는상황의 애니메이션이 아니라면
						{
							gScene->hierarchicalGameObjects.data()[1]->nowState = OTHER;
						}
						else // 가드가 아닌 동작이라면, 공격중이다.
						{
							gScene->hierarchicalGameObjects.data()[1]->nowState = ATTACK;
						}
					}
					else
					{
						gScene->hierarchicalGameObjects.data()[1]->nowState = MOVE;
					}
				}
			}
			break;
		}
	case ANIMATION_TYPE_PINGPONG:
		break;
	}

	if (m_pAnimationCallbackHandler)
	{
		void* pCallbackData = GetCallbackData();
		if (pCallbackData) m_pAnimationCallbackHandler->HandleCallback(pCallbackData);
	}
	}
}

XMFLOAT4X4 AnimationSet::GetSRT(int nBone)
{
	XMFLOAT4X4 xmf4x4Transform = Matrix4x4::Identity();
#ifdef _WITH_ANIMATION_SRT
	XMVECTOR S, R, T;
	for (auto i = 0; i < (m_nKeyFrameTranslations - 1); i++)
	{
		if ((m_pfKeyFrameTranslationTimes[i] <= m_fPosition) && (m_fPosition <= m_pfKeyFrameTranslationTimes[i + 1]))
		{
			float t = (m_fPosition - m_pfKeyFrameTranslationTimes[i]) / (m_pfKeyFrameTranslationTimes[i + 1] - m_pfKeyFrameTranslationTimes[i]);
			T = XMVectorLerp(XMLoadFloat3(&m_ppxmf3KeyFrameTranslations[i][nBone]), XMLoadFloat3(&m_ppxmf3KeyFrameTranslations[i + 1][nBone]), t);
			break;
		}
	}
	for (auto i = 0; i < (m_nKeyFrameScales - 1); i++)
	{
		if ((m_pfKeyFrameScaleTimes[i] <= m_fPosition) && (m_fPosition <= m_pfKeyFrameScaleTimes[i + 1]))
		{
			float t = (m_fPosition - m_pfKeyFrameScaleTimes[i]) / (m_pfKeyFrameScaleTimes[i + 1] - m_pfKeyFrameScaleTimes[i]);
			S = XMVectorLerp(XMLoadFloat3(&m_ppxmf3KeyFrameScales[i][nBone]), XMLoadFloat3(&m_ppxmf3KeyFrameScales[i + 1][nBone]), t);
			break;
		}
	}
	for (auto i = 0; i < (m_nKeyFrameRotations - 1); i++)
	{
		if ((m_pfKeyFrameRotationTimes[i] <= m_fPosition) && (m_fPosition <= m_pfKeyFrameRotationTimes[i + 1]))
		{
			float t = (m_fPosition - m_pfKeyFrameRotationTimes[i]) / (m_pfKeyFrameRotationTimes[i + 1] - m_pfKeyFrameRotationTimes[i]);
			R = XMQuaternionSlerp(XMLoadFloat4(&m_ppxmf4KeyFrameRotations[i][nBone]), XMLoadFloat4(&m_ppxmf4KeyFrameRotations[i + 1][nBone]), t);
			break;
		}
	}

	XMStoreFloat4x4(&xmf4x4Transform, XMMatrixAffineTransformation(S, XMVECTOR(), R, T));
#else   
	for (int i = 0; i < (m_nKeyFrames - 1); i++)
	{
		if ((m_pfKeyFrameTimes[i] <= m_fPosition) && (m_fPosition <= m_pfKeyFrameTimes[i + 1]))
		{
			float t = (m_fPosition - m_pfKeyFrameTimes[i]) / (m_pfKeyFrameTimes[i + 1] - m_pfKeyFrameTimes[i]);
			xmf4x4Transform = Matrix4x4::Interpolate(m_ppxmf4x4KeyFrameTransforms[i][nBone], m_ppxmf4x4KeyFrameTransforms[i + 1][nBone], t);
			break;
		}
	}
#endif
	return(xmf4x4Transform);
}

void AnimationSet::SetCallbackKeys(int nCallbackKeys)
{
	m_nCallbackKeys = nCallbackKeys;
	m_pCallbackKeys = new CALLBACKKEY[nCallbackKeys];
}

void AnimationSet::SetCallbackKey(int nKeyIndex, float fKeyTime, void* pData)
{
	m_pCallbackKeys[nKeyIndex].m_fTime = fKeyTime;
	m_pCallbackKeys[nKeyIndex].m_pCallbackData = pData;
}

void AnimationSet::SetAnimationCallbackHandler(AnimationCallbackHandler* pCallbackHandler)
{
	m_pAnimationCallbackHandler = pCallbackHandler;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
AnimationSets::AnimationSets(int nAnimationSets)
{
	m_nAnimationSets = nAnimationSets;
	m_pAnimationSets = new AnimationSet * [nAnimationSets];
}

AnimationSets::~AnimationSets()
{
	for (int i = 0; i < m_nAnimationSets; i++) SAFE_DELETE(m_pAnimationSets[i]);
	SAFE_DELETEARR(m_pAnimationSets);

	SAFE_DELETEARR(m_ppAnimatedBoneFrameCaches);
}

void AnimationSets::AddRef()
{ 
	m_nReferences++;
}

void AnimationSets::Release()
{
	if (--m_nReferences <= 0) delete this;
}

void AnimationSets::SetCallbackKeys(int nAnimationSet, int nCallbackKeys)
{
	m_pAnimationSets[nAnimationSet]->m_nCallbackKeys = nCallbackKeys;
	m_pAnimationSets[nAnimationSet]->m_pCallbackKeys = new CALLBACKKEY[nCallbackKeys];
}

void AnimationSets::SetCallbackKey(int nAnimationSet, int nKeyIndex, float fKeyTime, void* pData)
{
	m_pAnimationSets[nAnimationSet]->m_pCallbackKeys[nKeyIndex].m_fTime = fKeyTime;
	m_pAnimationSets[nAnimationSet]->m_pCallbackKeys[nKeyIndex].m_pCallbackData = pData;
}

void AnimationSets::SetAnimationCallbackHandler(int nAnimationSet, AnimationCallbackHandler* pCallbackHandler)
{
	m_pAnimationSets[nAnimationSet]->SetAnimationCallbackHandler(pCallbackHandler);
}