#include "framework.h"
#include "Timer.h"

//------------------------------
__int64 Timer::GetTime()
//------------------------------
// 현재 시간을 리턴하는 함수
{
	__int64 temp;
	// Performance Counter가 있다면 사용해서 현재 시간을 구함
	if (m_bHardwareHasPerformanceCounter)
		::QueryPerformanceCounter((LARGE_INTEGER*)&temp);
	// 없다면 timeGetTime()함수를 사용해서 현재 시간을 구함
	else
		temp = ::timeGetTime();
	return temp;
}

//------------------------------
float Timer::GetfTimeElapsed()
//------------------------------
// Frame 단위 Tick 수행 소요 시간을 리턴하는 함수
{
	return m_fTimeElapsed;
}

//------------------------------
float Timer::GetnTimeElapsed()
//------------------------------
// Timer 전체 수행 소요 시간을 리턴하는 함수
{
	return (m_nCurrentTime - m_nStartTime) * m_fTimeScale;
}

//------------------------------
unsigned long Timer::GetFrameRate(const LPTSTR& lpszString, const int& nCharacters) const
//------------------------------
// FPS 코드
//
{
	if (lpszString) {
		// 현재 프레임 레이트를 문자열로 변환하여 lpszString 버퍼에 쓰고 "FPS"와 결합
		_itow_s(m_nCurrentFrameRate, lpszString, nCharacters, 10);
		wcscat_s(lpszString, nCharacters, _T(" FPS)"));
	}

	return(m_nCurrentFrameRate);
}

//------------------------------
void Timer::Init()
//------------------------------
{
	// 변수 초기화
	m_nStartTime = m_nLastTime;
	m_nStopTime = 0;
	m_nCurrentTime = 0;
	m_nSampleCount = 0;
	m_nCurrentFrameRate = 0;
	m_nFramesPerSecond = 0;
	m_fFPSTimeElapsed = 0.0f;
	m_fTimeElapsed = 0.0f;
	m_bStopped = false;

	for (int i = 0; i < MAX_SAMPLE_COUNT; ++i)
		m_fFrameTime[i] = 0.0f;
}

//------------------------------
void Timer::Start()
//------------------------------
{
	if (m_bStopped) {
		// 고해상도 타이머(Performance Counter)의 주파수를 얻음과 동시에 존재 유무를 확인함
		if (::QueryPerformanceFrequency((LARGE_INTEGER*)&m_nPerformanceFrequency)) {
			m_bHardwareHasPerformanceCounter = TRUE;
			// 시작 시점의 CPU의 클럭 수를 얻음
			::QueryPerformanceCounter((LARGE_INTEGER*)&m_nLastTime);
			// 수행시간 (C 클럭 수)  = 끝 시점 (B 클럭 수) - 시작 시점 (A 클럭 수)

			// 시간 척도를 구함
			m_fTimeScale = 1.0f / m_nPerformanceFrequency;
		}
		else {
			// Performance Counter가 없으면 timeGetTime 함수로 milsec 단위 흐른 시간을 받아옴
			m_bHardwareHasPerformanceCounter = FALSE;
			m_nLastTime = ::timeGetTime();
			m_fTimeScale = 0.001f;	// 1.0f / 주파수(1000)
		}
		this->Init();
	}
}

//------------------------------
void Timer::Tick()
//------------------------------
{
	m_nCurrentTime = GetTime();

	// 마지막으로 이 함수를 호출한 이후 경과한 시간을 계산
	float fTimeElapsed = (m_nCurrentTime - m_nLastTime) * m_fTimeScale;

	// FPS lock이 0보다 크면 이 시간만큼 호출한 함수를 기다리게 함
	if (fLockFPS > 0.0f) {
		while (fTimeElapsed < (1.0f / ((fLockFPS > 0.0f) ? fLockFPS : 1.0f))) {
			if (m_bHardwareHasPerformanceCounter)
				::QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime);
			else
				m_nCurrentTime = ::timeGetTime();

			// 마지막으로 이 함수를 호출한 이후 경과한 시간을 계산
			fTimeElapsed = (m_nCurrentTime - m_nLastTime) * m_fTimeScale;
		}
	}

	// 현재 시간을 m_nLastTime에 저장
	m_nLastTime = m_nCurrentTime;

	// 마지막 프레임 처리 시간과 현재 프레임 처리 시간의 차이가 1초보다 작으면
	// 현재 프레임 처리 시간을 m_fFrameTime[0]에 저장
	if (fabsf(fTimeElapsed - m_fTimeElapsed) < 1.0f) {
		::memmove(&m_fFrameTime[1], m_fFrameTime, (MAX_SAMPLE_COUNT - 1) * sizeof(float));
		m_fFrameTime[0] = fTimeElapsed;
		if (m_nSampleCount < MAX_SAMPLE_COUNT)
			++m_nSampleCount;
	}

	// 초당 프레임 수를 1 증가시키고 현재 프레임 처리 시간을 누적하여 저장
	++m_nFramesPerSecond;
	m_fFPSTimeElapsed += fTimeElapsed;
	if (m_fFPSTimeElapsed > 1.0f) {
		m_nCurrentFrameRate = m_nFramesPerSecond;
		m_nFramesPerSecond = 0;
		m_fFPSTimeElapsed = 0.0f;
	}

	// 누적된 프레임 처리 시간의 평균을 구하여 프레임 처리 시간을 구함
	m_fTimeElapsed = 0.0f;
	for (ULONG i = 0; i < m_nSampleCount; ++i) {
		m_fTimeElapsed += m_fFrameTime[i];
	}
	if (m_nSampleCount > 0)
		m_fTimeElapsed /= m_nSampleCount;
}

//------------------------------
void Timer::Stop()
//------------------------------
{
	if (!m_bStopped) {
		m_nCurrentTime = GetTime();
		m_nStopTime = m_nCurrentTime;
		m_bStopped = true;
	}
}

//------------------------------
void Timer::Resume()
//------------------------------
{
	if (m_bStopped) {
		m_nLastTime = GetTime();
		m_bStopped = false;
	}
}

//------------------------------
void Timer::Reset()
//------------------------------
{
	m_nLastTime = GetTime();
	this->Init();
}