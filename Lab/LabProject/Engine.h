#pragma once

constexpr UINT FRAME_WIDTH = 800;
constexpr UINT FRAME_HEIGHT = 600;

struct CB_ENGINE_INFO
{
	float m_fCurrentTime = 0.f;
	float m_fElapsedTime = 0.f;
};

#include "Timer.h"
class Scene;
class Player;
class Camera;
class Server;

class Engine
{
public:
	Engine() = default;
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine&& operator=(Engine&&) = delete;
	~Engine() = default;
private:
	HINSTANCE hInstance = nullptr;
	HWND hWnd = nullptr;

	int wndClientWidth{FRAME_WIDTH};
	int wndClientHeight{FRAME_HEIGHT};

	IDXGIFactory4* factory = nullptr;
	IDXGISwapChain3* swapChain = nullptr;
	ID3D12Device* device = nullptr;

	bool bMsaa4xEnable = false;
	UINT nMsaa4xQualityLevels{ 0 };

	static const UINT swapChainBuffers{ 2 };
	UINT swapChainBufferIndex{ 0 };

	ID3D12Resource* swapChainBackBuffers[swapChainBuffers]{};
	ID3D12DescriptorHeap* rtvDescHeap = nullptr;
	UINT rtvDescIncrementSize{ 0 };

	ID3D12Resource* depthStencilBuffer = nullptr;
	ID3D12DescriptorHeap* dsvDescHeap = nullptr;
	UINT dsvDescIncrementSize{ 0 };

	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;

	ID3D12Fence* fence = nullptr;
	UINT64 fenceValues[swapChainBuffers]{ 0 };
	HANDLE fenceEvent{};

#if defined(_DEBUG)
	ID3D12Debug* debugController = nullptr;
#endif

	GameTimer m_GameTimer{};

	Scene* m_pScene = nullptr;
	Player* m_pPlayer = nullptr;
	Camera* m_pCamera = nullptr;

	POINT m_ptOldCursorPos{};

	_TCHAR frameRate[70]{ _T("AngerControlDisorder(") };

	ID3D12Resource* m_pd3dcbFrameworkInfo = nullptr;
	CB_ENGINE_INFO* m_pcbMappedFrameworkInfo = nullptr;

	Server* server = nullptr;
public:

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateCommandQueueAndList();

	void CreateRtvAndDsvDescriptorHeaps();

	void CreateRenderTargetViews();
	void CreateDepthStencilView();

	void ChangeSwapChainState();

	void CreateShaderVariables();
	void UpdateShaderVariables();
	void ReleaseShaderVariables();

	void BuildObjects();
	void Release();

	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();

	void WaitForGpuComplete();
	void MoveToNextFrame();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

#define PLAYER_SET_ANIMATION(input, animType, state, atkType) \
	if (pKeysBuffer[input] & 0xF0) \
	{ \
		m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, animType); \
		m_pPlayer->nowState = state; \
		m_pPlayer->attackType = atkType;