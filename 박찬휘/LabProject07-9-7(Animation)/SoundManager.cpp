#include "stdafx.h"
#include "SoundManager.h"

SoundManager::SoundManager(const char* path, bool loop)
{
	if (loop)
		result = FMOD_System_CreateSound(sound_system, path, FMOD_LOOP_NORMAL, nullptr, &sound);
	else
		result = FMOD_System_CreateSound(sound_system, path, FMOD_DEFAULT, nullptr, &sound);
	if (result != FMOD_OK) printf("Failed to CreateSound\n");
}

SoundManager::~SoundManager()
{
	/*if (sound_system) delete sound_system;
	if (sound) delete sound;
	if (channel) delete channel;*/
}

void SoundManager::Init()
{
	// Create FMOD System 
	result = FMOD_System_Create(&sound_system);
	if (result != FMOD_OK) printf("Failed to create fmod system\n");
	result = FMOD_System_Init(sound_system, 32, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK) printf("Faild to init fmod system\n");
}

void SoundManager::Release()
{
	FMOD_System_Close(sound_system);
	FMOD_System_Release(sound_system);
}

void SoundManager::Play()
{
	/*FMOD_CHANNEL_FREE  = -1,      For a channel index, FMOD chooses a free voice using the priority system. */
	/*FMOD_CHANNEL_REUSE = -2       For a channel index, re-use the channel handle that was passed in. */
	result = FMOD_System_PlaySound(sound_system, sound, nullptr, false, &channel);
	if (result != FMOD_OK) printf("Faild to PlaySound\n");
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
	if (this->volume + volume < SOUND_MAX)
		this->volume += volume;
	result = FMOD_Channel_SetVolume(channel, this->volume);
	if (result != FMOD_OK) printf("Faild to SetVolumeUp\n");
}

void SoundManager::VolumeDown(const float& volume)
{
	if (this->volume - volume > SOUND_MIN)
		this->volume -= volume;
	result = FMOD_Channel_SetVolume(channel, this->volume);
	if (result != FMOD_OK) printf("Faild to SetVolumeDown\n");
}

void SoundManager::Update()
{
	result = FMOD_Channel_IsPlaying(channel, &isPlaying);
	if (result != FMOD_OK) printf("Failed to Check IsPlaying\n");

	if (isPlaying) result = FMOD_System_Update(sound_system);
	if (result != FMOD_OK) printf("Failed to Update\n");
}
