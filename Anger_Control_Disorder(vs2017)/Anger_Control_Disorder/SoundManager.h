#pragma once

#include <fmod.h>

constexpr float SOUND_MAX = 1.0f;
constexpr float SOUND_MIN = 0.0f;
constexpr float SOUND_DEFAULT = 0.5f;
constexpr float SOUND_WEIGHT = 0.02f;

class SoundManager
{
public:
	SoundManager() = default;
	SoundManager(const char* path, bool loop);
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	~SoundManager() = default;

private:
	static FMOD_SYSTEM* soundsys;

	FMOD_SOUND* sound = nullptr;
	FMOD_CHANNEL* channel = nullptr;

	float volume = SOUND_DEFAULT;
	bool mbool = false;

public:
	static int Init();
	static int Release();

	int Play();
	int Pause();
	int Resume();
	int Stop();
	int VolumeUp();
	int VolumeDown();

	int Update();
};

