#pragma once
#include "Scene.h"
#include "SoundManager.h"
#include "Timer.h"

class Engine final
{
public:
	Engine() = default;
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	~Engine() = default;

private: /* DX Values */

	HINSTANCE Instance{ nullptr };
	HWND Wnd{ nullptr };

	int WndClientWidth{ FRAME_BUFFER_WIDTH };
	int WndClientHeight{ FRAME_BUFFER_HEIGHT };

	IDXGIFactory4* Factory{ nullptr };
	IDXGISwapChain3* SwapChain{ nullptr };
	ID3D12Device* Device{ nullptr };

	bool Msaa4xEnable{ false };
	UINT Msaa4xQualityLevels{ 0 };

	//Ω∫ø“√º¿Œ
	static const UINT SwapChainBuffers{ 2 };
	ID3D12Resource* SwapChainBackBuffers[SwapChainBuffers];
	ID3D12DescriptorHeap* RtvDescriptorHeap{ nullptr };
	UINT SwapChainBufferIndex{ 0 };
	UINT RtvDescriptorIncrementSize{ 0 };

	//±Ì¿ÃΩ∫≈ŸΩ«
	ID3D12Resource* DepthStencilBuffer{ nullptr };
	ID3D12DescriptorHeap* DsvDescriptorHeap{ nullptr };
	UINT DsvDescriptorIncrementSize{ 0 };

	//∏Ì∑…∏Ò∑œ
	ID3D12CommandAllocator* CommandAllocator{ nullptr };
	ID3D12CommandQueue* CommandQueue{ nullptr };
	ID3D12GraphicsCommandList* CommandList{ nullptr };

	//∆ÊΩ∫
	UINT64 FenceValues[SwapChainBuffers];
	ID3D12Fence* Fence{ nullptr };
	HANDLE FenceEvent{ nullptr };

#if defined(_DEBUG)
	ID3D12Debug* DebugController{ nullptr };
#endif

private: /* Engine Values */

	Scene* mScene{ nullptr };
	Timer* mTimer{ nullptr };
	bool isPaused = false;

	POINT mOldCursorPos{};
	_TCHAR mFrameRate[50];

public: /* Funciton */

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateCommandQueueAndList();

	void CreateRtvAndDsvDescriptorHeaps();

	void CreateRenderTargetViews();
	void CreateDepthStencilView();
	void CreateRenderTargetViewsAndDepthStencilView();

	void ChangeSwapChainState();

	void BuildObjects();
	void ReleaseObjects();

	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();

	virtual void CreateShaderVariables();
	virtual void UpdateShaderVariables();
	virtual void ReleaseShaderVariables();

	void OnProcessingMouseMessage(HWND hWnd, UINT MessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT MessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProecssingWindowMessage(HWND hWnd, UINT MessageID, WPARAM wParam, LPARAM lParam);

	void WaitForGpuComplete();
	void MoveToNextFrame();

};