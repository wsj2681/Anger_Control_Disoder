#include "stdafx.h"
#include "SoundManager.h"

FMOD_SYSTEM* SoundManager::system;

SoundManager::SoundManager(const char* path, bool isLoop)
{
	if (isLoop)
	{
		if (FMOD_System_CreateSound(system, path, FMOD_LOOP_NORMAL, nullptr, &sound) != FMOD_OK) cout << "Failed to CreateSound(" << path << ")" << endl;
	}
	else
	{
		if (FMOD_System_CreateSound(system, path, FMOD_DEFAULT, nullptr, &sound) != FMOD_OK) cout << "Failed to CreateSound(" << path << ")" << endl;
	}
}

SoundManager::~SoundManager()
{
	FMOD_System_Release(system);
}

void SoundManager::Init()
{
	// Create FMOD System
	if (FMOD_System_Create(&system) != FMOD_OK) cout << "Failed to Create FMOD System" << endl;
	if (FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, nullptr) != FMOD_OK) cout << "Failed to Init FMOD System" << endl;
}

void SoundManager::Release()
{
	FMOD_System_Close(system);
	FMOD_System_Release(system);
}

void SoundManager::Play()
{
	if (FMOD_System_PlaySound(system, sound, nullptr, false, &channel) != FMOD_OK) cout << "Failed to PlaySound" << endl;
}

void SoundManager::Resume()
{
	FMOD_Channel_SetPaused(channel, false);
}

void SoundManager::Stop()
{
	FMOD_Channel_SetPaused(channel, true);
}

void SoundManager::VolumeUp(const float& volume)
{
	if (this->volume + volume < SOUND_MAX) this->volume += volume;
	if (FMOD_Channel_SetVolume(channel, this->volume) != FMOD_OK) cout << "Failed to SetVolumeUp(" << volume << ")" << endl;
}

void SoundManager::VolumeDown(const float& volume)
{
	if (this->volume - volume > SOUND_MIN) this->volume -= volume;
	if (FMOD_Channel_SetVolume(channel, this->volume) != FMOD_OK) cout << "Failed to SetVolumeDown(" << volume << ")" << endl;
}

void SoundManager::Update()
{
	//if (FMOD_Channel_IsPlaying(channel, &isPlaying) != FMOD_OK) cout << "Failed to Check IsPlaying" << endl;
	if (isPlaying)
	{
		if (FMOD_System_Update(system) != FMOD_OK) cout << "Failed to Update" << endl;
	}
}
