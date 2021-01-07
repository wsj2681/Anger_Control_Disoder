#pragma once

class Device;
class Renderer;

class Engine final
{
public:

	Engine();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	~Engine();

private:

	Renderer* renderer;

public:

	void OnInit(HINSTANCE hInstance, HWND hWnd);
	void OnDestroy();
	void Update();
};

