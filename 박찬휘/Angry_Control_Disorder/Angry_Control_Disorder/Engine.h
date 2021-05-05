#pragma once
#include "Input.h"
#include "Device.h"

struct CB_ENGINE_INFO
{
	float totalTime = 0.f;
	float elapsedTime = 0.f;
};

class Engine final
{
public:
	Engine() = default;
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	~Engine() = default;

private:

	LPCWSTR applicationName = nullptr;
	HINSTANCE hInstance = nullptr;
	HWND hWnd = nullptr;

	Input* input = nullptr;
	Device* device = nullptr;

public:

	bool Init();
	void Run();
	bool FrameAdvance();
	void Release();
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

	Device* GetDevice();

private:

	bool InitWindow(int& windowWidth, int& windowHeight);

};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
static Engine* ApplicationHandle = nullptr;