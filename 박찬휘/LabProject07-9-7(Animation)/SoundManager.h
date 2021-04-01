#pragma once
#include <fmod.h>

// Constant Variables
constexpr float SOUND_MAX{ 1.0f };
constexpr float SOUND_MIN{ 0.0f };
constexpr float SOUND_DEFAULT{ 0.1f };


class SoundManager final
{
public:
	SoundManager() = delete;
	SoundManager(const char* path, bool loop);
	SoundManager(const SoundManager&) = default;
	SoundManager& operator=(const SoundManager&) = default;
	~SoundManager();
private:
	// Variables Which Uses Heap Memory
	FMOD_SYSTEM* sound_system{ nullptr };
	FMOD_SOUND* sound{ nullptr };
	FMOD_CHANNEL* channel{ nullptr };

	// General Variables
	FMOD_RESULT result{ FMOD_RESULT::FMOD_OK };
	FMOD_BOOL isPlaying{ false };
	float volume{ SOUND_DEFAULT };
public:
	void Init();
	void Release();
	void Play();
	void Resume();
	void Stop();
	void VolumeUp(const float& volume = 0.1f);
	void VolumeDown(const float& volume = 0.1f);
	void Update();
};