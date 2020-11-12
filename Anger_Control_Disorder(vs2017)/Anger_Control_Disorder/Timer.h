#pragma once

// Global Variables
const ULONG MAX_SAMPLE_COUNT = 50;	// 50회의 프레임 처리 시간을 누적하여 평균함
const float fLockFPS = 0.0f;		// FPS Lock 변수 Default = 0.0f

class Timer {
private:
	// Class Member Variables
	__int64 m_nPerformanceFrequency;	// 고해상도 타이머의 주파수

	float m_fTimeScale;		// 시간 척도
	float m_fTimeElapsed;	// Frame 단위 Tick 수행 소요 시간
	__int64 m_nLastTime;	// Frame 단위 Tick 수행 전의 시간
	__int64 m_nTimeElapsed;	// 타이머 수행 소요 시간
	__int64 m_nCurrentTime;	// 현재 시간
	__int64 m_nStartTime;	// 시작 시간
	__int64	m_nStopTime;	// 정지 시간

	bool m_bHardwareHasPerformanceCounter;	// HW Performance Count 소유 여부 담는 변수
	bool m_bStopped;						// 현재 타이머가 멈췄는지를 담는 변수

	float m_fFrameTime[MAX_SAMPLE_COUNT];	// Frame 단위 Tick 수행 소요 시간을 저장하는 변수
	ULONG m_nSampleCount;					// Frame 샘플의 갯수 - 현재 최대 50개로 지정됨

	unsigned long m_nFramesPerSecond;		// FPS 저장하는 변수
	float m_fFPSTimeElapsed;				// FPS를 누적하여 저장하는 변수
public:
	unsigned long m_nCurrentFrameRate;		// 현재 프레임 레이트를 저장하는 변수
public:
	// Functions
	// Special Member Function
	Timer() { this->Start(); }
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;
	~Timer() = default;

	// Get & Set	
	__int64 GetTime();			// 현재 시간을 리턴하는 함수
	float GetfTimeElapsed();	// Frame 단위 Tick 수행 소요 시간을 리턴하는 함수
	float GetnTimeElapsed();	// Timer 전체 수행 소요 시간을 리턴하는 함수
	unsigned long GetFrameRate(const LPTSTR& lpszString, const int& nCharacters) const; // FPS 코드

	// Basic Function
	void Init();	// 타이머 초기화 함수
	void Start();	// 타이머 시작 함수
	void Tick();	// 타이머 1회 수행 함수
	void Stop();	// 타이머 정지 함수
	void Resume();	// 타이머 재개 함수
	void Reset();	// 타이머 리셋 함수
};