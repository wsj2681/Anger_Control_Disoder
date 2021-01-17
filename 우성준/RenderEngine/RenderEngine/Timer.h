#pragma once
//-----------------------------------------------------------------------------
// File: Timer.h
//-----------------------------------------------------------------------------

const ULONG MAX_SAMPLE_COUNT = 50; // Maximum frame time sample count

class Timer
{
public:
	Timer();
	virtual ~Timer();

	void Tick(float fLockFPS = 0.0f);
	void Start();
	void Stop();
	void Reset();

	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);
	float GetTimeElapsed();
	float GetTotalTime();

private:
	double							timeScale;
	float							timeElapsed;

	__int64							basePerformaceCounter;
	__int64							pausedPerformanceCounter;
	__int64							stopPerformanceCounter;
	__int64							currentPerformanceCounter;
	__int64							lastPerformanceCounter;

	__int64							performanceFrequencyPerSec;

	float							frameTime[MAX_SAMPLE_COUNT];
	ULONG							sampleCount;

	unsigned long					currentFrameRate;
	unsigned long					framesPerSecond;
	float							FPSTimeElapsed;

	bool							stopped;
};
