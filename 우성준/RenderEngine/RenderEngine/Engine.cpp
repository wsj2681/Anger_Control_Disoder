#include "framework.h"
#include "Device.h"
#include "Renderer.h"
#include "Engine.h"

Engine::Engine()
{
	this->device = new Device();
	this->renderer = new Renderer(device->GetCommandAllocator(), device->GetCommandQueue(), device->GetCommandList());
}

Engine::~Engine()
{
}
