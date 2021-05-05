#include "framework.h"
#include "Engine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Engine* engine;

	engine = new Engine;
	if (!engine)
	{
		return 0;
	}

	if (engine->Init())
	{
		engine->Run();
	}

	engine->Release();
	delete engine;
	engine = nullptr;

	return 0;
}