#include "stdafx.h"
#include "CallBack.h"

#define _WITH_DEBUG_CALLBACK_DATA

void CSoundCallbackHandler::HandleCallback(void* pCallbackData)
{
	_TCHAR* pWavName = (_TCHAR*)pCallbackData;
#ifdef _WITH_DEBUG_CALLBACK_DATA
	TCHAR pstrDebug[256] = { 0 };
	_stprintf_s(pstrDebug, 256, _T("%s\n"), pWavName);
	OutputDebugString(pstrDebug);
#endif
#ifdef _WITH_SOUND_RESOURCE
	PlaySound(pWavName, ::ghAppInstance, SND_RESOURCE | SND_ASYNC);
#else
	PlaySound(pWavName, nullptr, SND_FILENAME | SND_ASYNC);
#endif
}