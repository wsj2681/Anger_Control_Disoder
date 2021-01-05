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
	device->OnInit();
	
	this->renderer = new Renderer(device->GetCommandAllocator(), device->GetCommandQueue(), device->GetCommandList());
	renderer->OnInit(hInstance, hWnd, device->GetDevice());
}

void Engine::OnDestroy()
{
}

void Engine::Update()
{
}
