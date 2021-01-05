#include "framework.h"
#include "Device.h"
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
	this->device = new Device();
	this->renderer = new Renderer(device->GetCommandAllocator(), device->GetCommandQueue(), device->GetCommandList());
	device->OnInit();
	renderer->OnInit(hInstance, hWnd);
}

void Engine::OnDestroy()
{
}

void Engine::Update()
{
}
