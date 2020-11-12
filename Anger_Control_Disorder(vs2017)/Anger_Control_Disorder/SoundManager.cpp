#include "SoundManager.h"

FMOD_SYSTEM* SoundManager::soundsys;

SoundManager::SoundManager(const char* path, bool loop)
{
	this->Init();

	if (loop)
	{
		FMOD_System_CreateSound(soundsys, path, FMOD_LOOP_NORMAL, 0, &sound);
	}
	else
	{
		FMOD_System_CreateSound(soundsys, path, FMOD_DEFAULT, 0, &sound);

	}
}

int SoundManager::Init()
{
	FMOD_System_Create(&soundsys);
	FMOD_System_Init(soundsys, 32, FMOD_INIT_NORMAL, nullptr);

	return 0;
}

int SoundManager::Release()
{
	FMOD_System_Close(soundsys);
	FMOD_System_Release(soundsys);

	return 0;
}

int SoundManager::Play()
{
	FMOD_System_PlaySound(soundsys, sound, nullptr, false, &channel);
	return 0;
}

int SoundManager::Pause()
{
	FMOD_Channel_SetPaused(channel, true);
	return 0;
}

int SoundManager::Resume()
{
	FMOD_Channel_SetPaused(channel, false);
	return 0;
}

int SoundManager::Stop()
{
	FMOD_Channel_Stop(channel);
	return 0;
}

int SoundManager::VolumeUp()
{
	if (this->volume < SOUND_MAX)
	{
		this->volume += SOUND_WEIGHT;
	}

	FMOD_Channel_SetVolume(channel, this->volume);

	return 0;
}

int SoundManager::VolumeDown()
{
	if (this->volume > SOUND_MIN)
	{
		this->volume -= SOUND_WEIGHT;
	}

	FMOD_Channel_SetVolume(channel, this->volume);

	return 0;
}

int SoundManager::Update()
{
	FMOD_Channel_IsPlaying(channel, (FMOD_BOOL*)this->mbool);
	FMOD_System_Update(soundsys);
	return 0;
}
