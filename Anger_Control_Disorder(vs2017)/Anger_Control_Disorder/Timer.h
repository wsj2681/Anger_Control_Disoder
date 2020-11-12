#pragma once

// Global Variables
const ULONG MAX_SAMPLE_COUNT = 50;	// 50ȸ�� ������ ó�� �ð��� �����Ͽ� �����
const float fLockFPS = 0.0f;		// FPS Lock ���� Default = 0.0f

class Timer {
private:
	// Class Member Variables
	__int64 m_nPerformanceFrequency;	// ���ػ� Ÿ�̸��� ���ļ�

	float m_fTimeScale;		// �ð� ô��
	float m_fTimeElapsed;	// Frame ���� Tick ���� �ҿ� �ð�
	__int64 m_nLastTime;	// Frame ���� Tick ���� ���� �ð�
	__int64 m_nTimeElapsed;	// Ÿ�̸� ���� �ҿ� �ð�
	__int64 m_nCurrentTime;	// ���� �ð�
	__int64 m_nStartTime;	// ���� �ð�
	__int64	m_nStopTime;	// ���� �ð�

	bool m_bHardwareHasPerformanceCounter;	// HW Performance Count ���� ���� ��� ����
	bool m_bStopped;						// ���� Ÿ�̸Ӱ� ��������� ��� ����

	float m_fFrameTime[MAX_SAMPLE_COUNT];	// Frame ���� Tick ���� �ҿ� �ð��� �����ϴ� ����
	ULONG m_nSampleCount;					// Frame ������ ���� - ���� �ִ� 50���� ������

	unsigned long m_nFramesPerSecond;		// FPS �����ϴ� ����
	float m_fFPSTimeElapsed;				// FPS�� �����Ͽ� �����ϴ� ����
public:
	unsigned long m_nCurrentFrameRate;		// ���� ������ ����Ʈ�� �����ϴ� ����
public:
	// Functions
	// Special Member Function
	Timer() { this->Start(); }
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;
	~Timer() = default;

	// Get & Set	
	__int64 GetTime();			// ���� �ð��� �����ϴ� �Լ�
	float GetfTimeElapsed();	// Frame ���� Tick ���� �ҿ� �ð��� �����ϴ� �Լ�
	float GetnTimeElapsed();	// Timer ��ü ���� �ҿ� �ð��� �����ϴ� �Լ�
	unsigned long GetFrameRate(const LPTSTR& lpszString, const int& nCharacters) const; // FPS �ڵ�

	// Basic Function
	void Init();	// Ÿ�̸� �ʱ�ȭ �Լ�
	void Start();	// Ÿ�̸� ���� �Լ�
	void Tick();	// Ÿ�̸� 1ȸ ���� �Լ�
	void Stop();	// Ÿ�̸� ���� �Լ�
	void Resume();	// Ÿ�̸� �簳 �Լ�
	void Reset();	// Ÿ�̸� ���� �Լ�
};