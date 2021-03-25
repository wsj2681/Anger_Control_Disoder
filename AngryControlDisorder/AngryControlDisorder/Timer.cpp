#include "framework.h"
#include "Timer.h"

Timer::Timer()
{
	::QueryPerformanceFrequency((LARGE_INTEGER*)&performanceFrequencyPerSec);
	::QueryPerformanceCounter((LARGE_INTEGER*)&lastPerformanceCounter);
	timeScale = 1.0 / (double)performanceFrequencyPerSec;

	basePerformanceCounter = lastPerformanceCounter;
}

Timer::~Timer()
{
}

void Timer::Tick(float fLockFPS)
{
	if (stopped)
	{
		timeElapsed = 0.0f;
		return;
	}
	float fTimeElapsed;

	::QueryPerformanceCounter((LARGE_INTEGER*)&currentPerformanceCounter);
	fTimeElapsed = float((currentPerformanceCounter - lastPerformanceCounter) * timeScale);

	if (fLockFPS > 0.0f)
	{
		while (fTimeElapsed < (1.0f / fLockFPS))
		{
			::QueryPerformanceCounter((LARGE_INTEGER*)&currentPerformanceCounter);
			fTimeElapsed = float((currentPerformanceCounter - lastPerformanceCounter) * timeScale);
		}
	}

	lastPerformanceCounter = currentPerformanceCounter;

	if (fabsf(fTimeElapsed - timeElapsed) < 1.0f)
	{
		::memmove(&frameTime[1], frameTime, (MAX_SAMPLE_COUNT - 1) * sizeof(float));
		frameTime[0] = fTimeElapsed;
		if (sampleCount < MAX_SAMPLE_COUNT) 
		{
			sampleCount++;
		}
	}

	framesPerSecond++;
	FPSTimeElapsed += fTimeElapsed;
	if (FPSTimeElapsed > 1.0f)
	{
		currentFrameRate = framesPerSecond;
		framesPerSecond = 0;
		FPSTimeElapsed = 0.0f;
	}

	timeElapsed = 0.0f;
	for (ULONG i = 0; i < sampleCount; i++)
	{
		timeElapsed += frameTime[i];
	}
	if (sampleCount > 0)
	{
		timeElapsed /= sampleCount;
	}
}

unsigned long Timer::GetFrameRate(LPTSTR lpszString, int nCharacters)
{
	if (lpszString)
	{
		_itow_s(currentFrameRate, lpszString, nCharacters, 10);
		wcscat_s(lpszString, nCharacters, _T(" FPS)"));
	}

	return currentFrameRate;
}

float Timer::GetTimeElapsed()
{
	return timeElapsed;
}

float Timer::GetTotalTime()
{
	if (stopped) return(float(((stopPerformanceCounter - pausedPerformanceCounter) - basePerformanceCounter) * timeScale));
	return float(((currentPerformanceCounter - pausedPerformanceCounter) - basePerformanceCounter) * timeScale);
}

void Timer::Reset()
{
	__int64 nPerformanceCounter;
	::QueryPerformanceCounter((LARGE_INTEGER*)&nPerformanceCounter);

	basePerformanceCounter = nPerformanceCounter;
	lastPerformanceCounter = nPerformanceCounter;
	stopPerformanceCounter = 0;
	stopped = false;
}

void Timer::Start()
{
	__int64 nPerformanceCounter;
	::QueryPerformanceCounter((LARGE_INTEGER*)&nPerformanceCounter);
	if (stopped)
	{
		pausedPerformanceCounter += (nPerformanceCounter - stopPerformanceCounter);
		lastPerformanceCounter = nPerformanceCounter;
		stopPerformanceCounter = 0;
		stopped = false;
	}
}

void Timer::Stop()
{
	if (!stopped)
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&stopPerformanceCounter);
		stopped = true;
	}
}
