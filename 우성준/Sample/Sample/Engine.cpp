#include "framework.h"
#include "Engine.h"
#include "Renderer.h"

bool Engine::OnInit(HINSTANCE hInstance, HWND hWnd)
{
	renderer = new Renderer(hInstance, hWnd);
	return true;
}

void Engine::OnDestroy()
{
	if (renderer)
	{
		renderer->OnDestroy();
		delete renderer;
		renderer = nullptr;
	}
}

void Engine::Update()
{
	renderer->Render();
}
