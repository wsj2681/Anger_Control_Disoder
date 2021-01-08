#pragma once

class Renderer;

class Engine final
{
public:

	Engine() = default;
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	~Engine() = default;

private:

	Renderer* renderer;

private:


public:

	bool OnInit(HINSTANCE hInstance, HWND hWnd);
	void OnDestroy();
	void Update();

};

