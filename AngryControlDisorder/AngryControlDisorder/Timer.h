#pragma once

constexpr ULONG MAX_SAMPLE_COUNT = 50;

class Timer final
{
public:
	Timer();
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;
	~Timer();

private:
	double							timeScale = 0.f;
	float							timeElapsed = 0.f;

	__int64							basePerformanceCounter = 0;
	__int64							pausedPerformanceCounter = 0;
	__int64							stopPerformanceCounter = 0;
	__int64							currentPerformanceCounter = 0;
	__int64							lastPerformanceCounter = 0;

	__int64							performanceFrequencyPerSec = 0;

	float							frameTime[MAX_SAMPLE_COUNT];
	ULONG							sampleCount = 0;

	unsigned long					currentFrameRate = 0;
	unsigned long					framesPerSecond = 0;
	float							FPSTimeElapsed = 0.f;

	bool							stopped = false;
public:

	void Tick(float fLockFPS = 0.0f);
	void Start();
	void Stop();
	void Reset();

	unsigned long GetFrameRate(LPTSTR lpszString = nullptr, int nCharacters = 0);
	float GetTimeElapsed();
	float GetTotalTime();
};

