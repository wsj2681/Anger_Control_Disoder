#include "framework.h"
#include "Timer.h"

//------------------------------
__int64 Timer::GetTime()
//------------------------------
// ���� �ð��� �����ϴ� �Լ�
{
	__int64 temp;
	// Performance Counter�� �ִٸ� ����ؼ� ���� �ð��� ����
	if (m_bHardwareHasPerformanceCounter)
		::QueryPerformanceCounter((LARGE_INTEGER*)&temp);
	// ���ٸ� timeGetTime()�Լ��� ����ؼ� ���� �ð��� ����
	else
		temp = ::timeGetTime();
	return temp;
}

//------------------------------
float Timer::GetfTimeElapsed()
//------------------------------
// Frame ���� Tick ���� �ҿ� �ð��� �����ϴ� �Լ�
{
	return m_fTimeElapsed;
}

//------------------------------
float Timer::GetnTimeElapsed()
//------------------------------
// Timer ��ü ���� �ҿ� �ð��� �����ϴ� �Լ�
{
	return (m_nCurrentTime - m_nStartTime) * m_fTimeScale;
}

//------------------------------
unsigned long Timer::GetFrameRate(const LPTSTR& lpszString, const int& nCharacters) const
//------------------------------
// FPS �ڵ�
//
{
	if (lpszString) {
		// ���� ������ ����Ʈ�� ���ڿ��� ��ȯ�Ͽ� lpszString ���ۿ� ���� "FPS"�� ����
		_itow_s(m_nCurrentFrameRate, lpszString, nCharacters, 10);
		wcscat_s(lpszString, nCharacters, _T(" FPS)"));
	}

	return(m_nCurrentFrameRate);
}

//------------------------------
void Timer::Init()
//------------------------------
{
	// ���� �ʱ�ȭ
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
		// ���ػ� Ÿ�̸�(Performance Counter)�� ���ļ��� ������ ���ÿ� ���� ������ Ȯ����
		if (::QueryPerformanceFrequency((LARGE_INTEGER*)&m_nPerformanceFrequency)) {
			m_bHardwareHasPerformanceCounter = TRUE;
			// ���� ������ CPU�� Ŭ�� ���� ����
			::QueryPerformanceCounter((LARGE_INTEGER*)&m_nLastTime);
			// ����ð� (C Ŭ�� ��)  = �� ���� (B Ŭ�� ��) - ���� ���� (A Ŭ�� ��)

			// �ð� ô���� ����
			m_fTimeScale = 1.0f / m_nPerformanceFrequency;
		}
		else {
			// Performance Counter�� ������ timeGetTime �Լ��� milsec ���� �帥 �ð��� �޾ƿ�
			m_bHardwareHasPerformanceCounter = FALSE;
			m_nLastTime = ::timeGetTime();
			m_fTimeScale = 0.001f;	// 1.0f / ���ļ�(1000)
		}
		this->Init();
	}
}

//------------------------------
void Timer::Tick()
//------------------------------
{
	m_nCurrentTime = GetTime();

	// ���������� �� �Լ��� ȣ���� ���� ����� �ð��� ���
	float fTimeElapsed = (m_nCurrentTime - m_nLastTime) * m_fTimeScale;

	// FPS lock�� 0���� ũ�� �� �ð���ŭ ȣ���� �Լ��� ��ٸ��� ��
	if (fLockFPS > 0.0f) {
		while (fTimeElapsed < (1.0f / ((fLockFPS > 0.0f) ? fLockFPS : 1.0f))) {
			if (m_bHardwareHasPerformanceCounter)
				::QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime);
			else
				m_nCurrentTime = ::timeGetTime();

			// ���������� �� �Լ��� ȣ���� ���� ����� �ð��� ���
			fTimeElapsed = (m_nCurrentTime - m_nLastTime) * m_fTimeScale;
		}
	}

	// ���� �ð��� m_nLastTime�� ����
	m_nLastTime = m_nCurrentTime;

	// ������ ������ ó�� �ð��� ���� ������ ó�� �ð��� ���̰� 1�ʺ��� ������
	// ���� ������ ó�� �ð��� m_fFrameTime[0]�� ����
	if (fabsf(fTimeElapsed - m_fTimeElapsed) < 1.0f) {
		::memmove(&m_fFrameTime[1], m_fFrameTime, (MAX_SAMPLE_COUNT - 1) * sizeof(float));
		m_fFrameTime[0] = fTimeElapsed;
		if (m_nSampleCount < MAX_SAMPLE_COUNT)
			++m_nSampleCount;
	}

	// �ʴ� ������ ���� 1 ������Ű�� ���� ������ ó�� �ð��� �����Ͽ� ����
	++m_nFramesPerSecond;
	m_fFPSTimeElapsed += fTimeElapsed;
	if (m_fFPSTimeElapsed > 1.0f) {
		m_nCurrentFrameRate = m_nFramesPerSecond;
		m_nFramesPerSecond = 0;
		m_fFPSTimeElapsed = 0.0f;
	}

	// ������ ������ ó�� �ð��� ����� ���Ͽ� ������ ó�� �ð��� ����
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