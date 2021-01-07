#include "framework.h"
#include "Renderer.h"
#include "Engine.h"

Engine::Engine()
{
}

Engine::~Engine()
{
}

void Engine::OnInit(HINSTANCE hInstance, HWND hWnd)
{

	this->renderer = new Renderer();
	renderer->OnInit(hInstance, hWnd);
}

void Engine::OnDestroy()
{
}

void Engine::Update()
{
	renderer->Render();
}
