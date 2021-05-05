#include "framework.h"
#include "Engine.h"

bool Engine::Init()
{
	int screenWidth = 0;
	int screenHeight = 0;

	if (!InitWindow(screenWidth, screenHeight))
	{
		return false;
	}

	input = new Input;
	input->Init();

	device = new Device();
	if (!device->Init(hInstance, hWnd))
	{
		return false;
	}

	return true;
}

void Engine::Run()
{
	bool done = false;

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!FrameAdvance())
			{
				done = true;
			}
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		
	}
}

bool Engine::FrameAdvance()
{
	if (input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	if (!device->FrameAdvance())
	{
		return false;
	}

	return true;
}

void Engine::Release()
{
	RELEASE(device);
	RELEASE(input);
}

LRESULT Engine::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_KEYDOWN:
		{
			input->KeyDown((unsigned int)wparam);
			device->OnProcessingKeyboardMessage(hwnd, umsg, wparam, lparam);
			return 0;
		}
		case WM_KEYUP:
		{
			input->KeyUp((unsigned int)wparam);
			device->OnProcessingKeyboardMessage(hwnd, umsg, wparam, lparam);
			return 0;
		}

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
		{
			device->OnProcessingMouseMessage(hwnd, umsg, wparam, lparam);
			return 0;
		}
		
		
		

		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

Device* Engine::GetDevice()
{
	return this->device;
}

bool Engine::InitWindow(int& windowWidth, int& windowHeight)
{
	ApplicationHandle = this;
	hInstance = GetModuleHandle(nullptr);
	applicationName = L"Engine";

	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	windowWidth = GetSystemMetrics(SM_CXSCREEN);
	windowHeight = GetSystemMetrics(SM_CYSCREEN);

	DEVMODE dmScreenSettings;
	int posX, posY;
	if (false)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)windowWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)windowHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		windowWidth = 1280;
		windowHeight = 720;

		posX = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;
	}

	hWnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

	if (ShowWindow(hWnd, SW_SHOW))
	{
		return false;
	}
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	ShowCursor(false);

	return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umsg, wparam, lparam);
		}
	}
}
