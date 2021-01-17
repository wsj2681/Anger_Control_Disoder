#pragma once

#include "Timer.h"

struct CB_FRAMEWORK_INFO
{
	float					m_fCurrentTime;
	float					m_fElapsedTime;
};

class Engine final
{
public:

	Engine() = default;
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	~Engine() = default;

private:

	HINSTANCE hInstance = nullptr;
	HWND hWnd = nullptr;

	int	m_nWndClientWidth;
	int	m_nWndClientHeight;

	IDXGIFactory4* factory4 = nullptr;
	IDXGISwapChain3* swapChain3 = nullptr;
	ID3D12Device* device = nullptr;

	bool m_bMsaa4xEnable = false;
	UINT m_nMsaa4xQualityLevels = 0;

	static const UINT m_nSwapChainBuffers = 2;
	UINT m_nSwapChainBufferIndex;

	ID3D12Resource* swapChainBackBuffers[m_nSwapChainBuffers];
	ID3D12DescriptorHeap* rtvDescriptorHeap = nullptr;
	UINT rtvDescIncrementSize = 0;

	ID3D12Resource* depthStencilBuffer = nullptr;
	ID3D12DescriptorHeap* dsvDescriptorHeap = nullptr;
	UINT dsvDescIncrementSize = 0;

	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;

	ID3D12Fence* fence = nullptr;
	UINT64 m_nFenceValues[m_nSwapChainBuffers]{};
	HANDLE m_hFenceEvent = nullptr;

#if defined(_DEBUG)
	ID3D12Debug* m_pd3dDebugController = nullptr;
#endif

	Timer gameTimer;

	POINT oldCursorPos{};

	_TCHAR frameRate[70]{};

	ID3D12Resource* frameworkInfo = nullptr;
	CB_FRAMEWORK_INFO* mappedFrameworkInfo = nullptr;

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

	void BuildObjects();
	void ReleaseObjects();

	void ProcessInput();
	void UpdatesObjects();
	void FrameAdvance();

	void WaitForGpuComplete();
	void MoveToNextFrame();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void CreateShaderVariables();
	void UpdateShaderVariables();
	void ReleaseShaderVariables();

};
