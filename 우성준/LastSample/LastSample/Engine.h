#pragma once

#define FRAME_BUFFER_WIDTH		1080
#define FRAME_BUFFER_HEIGHT		720

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

	int wndClientWidth = FRAME_BUFFER_WIDTH;
	int wndClientHeight = FRAME_BUFFER_HEIGHT;

	IDXGIFactory4* factory = nullptr;
	IDXGISwapChain3* swapChain = nullptr;
	ID3D12Device* device = nullptr;

	bool msaa4xEnable = false;
	UINT msaa4xQualityLevels = 0;

	static const UINT swapChainBuffers = 2;
	UINT swapChainBufferIndex = 0;

	ID3D12Resource* swapChainBackBuffers[swapChainBuffers];
	ID3D12DescriptorHeap* rtvDescHeap = nullptr;

	ID3D12Resource* depthStencilBuffer = nullptr;
	ID3D12DescriptorHeap* dsvDescHeap = nullptr;

	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;

	ID3D12Fence* fence = nullptr;
	UINT fenceValues[swapChainBuffers];
	HANDLE fenceEvent = nullptr;

#if defined(_DEBUG)
	ID3D12Debug* debugController = nullptr;
#endif

	FbxManager* fbxSdkManager = nullptr;
	FbxScene* fbxScene = nullptr;

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
	void AnimateObjects();
	void FrameAdvance();

	void WaitForGpuComplete();
	void MoveToNextFrame();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void CreateFbxSdkManager();
};

