#pragma once

#define FRAME_BUFFER_WIDTH		800
#define FRAME_BUFFER_HEIGHT		600

#include "Timer.h"
#include "Player.h"
#include "Scene.h"

////SEVER////
#include "Server.h"

struct CB_ENGINE_INFO
{
	float m_fCurrentTime = 0.f;
	float m_fElapsedTime = 0.f;
};

class Engine
{
public:

	Engine();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	~Engine();

private:
	HINSTANCE hInstance = nullptr;
	HWND hWnd = nullptr;

	int wndClientWidth;
	int wndClientHeight;

	IDXGIFactory4* factory = nullptr;
	IDXGISwapChain3* swapChain = nullptr;
	ID3D12Device* device = nullptr;

	bool bMsaa4xEnable = false;
	UINT nMsaa4xQualityLevels = 0;

	static const UINT swapChainBuffers = 2;
	UINT swapChainBufferIndex = 0;

	ID3D12Resource* swapChainBackBuffers[swapChainBuffers];
	ID3D12DescriptorHeap* rtvDescHeap = nullptr;
	UINT rtvDescIncrementSize;

	ID3D12Resource* depthStencilBuffer = nullptr;
	ID3D12DescriptorHeap* dsvDescHeap = nullptr;
	UINT dsvDescIncrementSize;

	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12CommandQueue* commandQueue = NULL;
	ID3D12GraphicsCommandList* commandList = nullptr;

	ID3D12Fence* fence = nullptr;
	UINT64						fenceValues[swapChainBuffers];
	HANDLE						fenceEvent;

#if defined(_DEBUG)
	ID3D12Debug* debugController;
#endif

	CGameTimer m_GameTimer;

	Scene* m_pScene = nullptr;
	Player* m_pPlayer = nullptr;
	Camera* m_pCamera = nullptr;

	POINT m_ptOldCursorPos;

	_TCHAR						frameRate[70];

	ID3D12Resource* m_pd3dcbFrameworkInfo = nullptr;
	CB_ENGINE_INFO* m_pcbMappedFrameworkInfo = nullptr;

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
	void ReleaseObjects();

	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();

	void WaitForGpuComplete();
	void MoveToNextFrame();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	///////////server/////////////
	Server* server = NULL;

	/////////////////////////////
};

