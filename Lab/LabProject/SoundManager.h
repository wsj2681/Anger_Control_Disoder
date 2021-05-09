#pragma once

#include <fmod.h>

constexpr float SOUND_MAX{ 1.0f };
constexpr float SOUND_MIN{ 0.0f };
constexpr float SOUND_DEFAULT{ 0.001f };

class SoundManager final
{
public:
	SoundManager() = default;
	SoundManager(const char* path, bool isLoop = false);
	~SoundManager();
private:
	static FMOD_SYSTEM* system;

	FMOD_SOUND* sound{ nullptr };
	FMOD_CHANNEL* channel{ nullptr };

	FMOD_BOOL isPlaying{ false };
	float volume{ 0.1f };
public:
	static void Init();
	static void Release();
	void Play();
	void Resume();
	void Stop();
	void VolumeUp(const float& volume = 0.1f);
	void VolumeDown(const float& volume = 0.1f);
	void Update();
};