#pragma once
#include "framework.h"

const ULONG MAX_SAMPLE_COUNT = 50;

class CGameTimer {
	bool	m_bHardwareHasPerformanceCounter;	// 컴퓨터가 Performance Counter를 가지고 있는지
	float	m_fTimeScale;						//Scale Counter의 양
	float	m_fTimeElapsed;						//마지막 프레임 이후 지난 시간
	__int64 m_nCurrentTime;						//현재 시간
	__int64 m_nLastTime;						//마지막 프레임 시간
	__int64 m_nPerformanceFrequency;			//컴퓨터의 Performance Frequency
	float	m_fFrameTime[MAX_SAMPLE_COUNT];		//프레임 시간을 누적하기 위한 배열
	
	ULONG			m_nSampleCount		{ 0 };		//누적된 프레임 횟수
	unsigned long	m_nCurrentFrameRate	{ 0 };		//현재 프레임 레이트
	unsigned long	m_nFramesPerSecond	{ 0 };		//초당 프레임 수
	float			m_fFPSTimeElapsed	{ 0.f };	//프레임 레이트 계산 소요시간

public:
	CGameTimer();
	virtual ~CGameTimer();

	void Tick(float fLockFPS = 0.0f);//타이머 시간 갱신 함수
	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);//프레임 레이트 반환 함수
	float GetTimeElapsed();//프레임 평균 경과시간 반환함수
	void Reset();
};

