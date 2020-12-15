#pragma once
#include "framework.h"

const ULONG MAX_SAMPLE_COUNT = 50;

class CGameTimer {
	bool	m_bHardwareHasPerformanceCounter;	// ��ǻ�Ͱ� Performance Counter�� ������ �ִ���
	float	m_fTimeScale;						//Scale Counter�� ��
	float	m_fTimeElapsed;						//������ ������ ���� ���� �ð�
	__int64 m_nCurrentTime;						//���� �ð�
	__int64 m_nLastTime;						//������ ������ �ð�
	__int64 m_nPerformanceFrequency;			//��ǻ���� Performance Frequency
	float	m_fFrameTime[MAX_SAMPLE_COUNT];		//������ �ð��� �����ϱ� ���� �迭
	
	ULONG			m_nSampleCount		{ 0 };		//������ ������ Ƚ��
	unsigned long	m_nCurrentFrameRate	{ 0 };		//���� ������ ����Ʈ
	unsigned long	m_nFramesPerSecond	{ 0 };		//�ʴ� ������ ��
	float			m_fFPSTimeElapsed	{ 0.f };	//������ ����Ʈ ��� �ҿ�ð�

public:
	CGameTimer();
	virtual ~CGameTimer();

	void Tick(float fLockFPS = 0.0f);//Ÿ�̸� �ð� ���� �Լ�
	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);//������ ����Ʈ ��ȯ �Լ�
	float GetTimeElapsed();//������ ��� ����ð� ��ȯ�Լ�
	void Reset();
};

