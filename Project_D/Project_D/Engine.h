// Engine : 다이렉트X 기반 프레임워크 입니다.

#pragma once

class Scene;
class Camera;


class Engine
{
public:
	Engine() = default;
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	~Engine() = default;

private:
	HINSTANCE hInstance = nullptr;
	HWND hWnd = nullptr;

	IDXGIFactory4* factory = nullptr;
	ID3D12Device* device = nullptr;

	bool Msaa4xEnable = false;
	UINT Msaa4xQualityLevels = 0;

	IDXGISwapChain3* swapChain = nullptr;
	static const UINT swapChainBuffers = 2;
	UINT swapChainBufferIndex = 0;

	ID3D12Resource* renderTargetBuffers[swapChainBuffers] = { nullptr, nullptr };
	ID3D12DescriptorHeap* renderTargetDescHeap = nullptr;
	UINT renderTargetIncrementSize = 0;

	ID3D12Resource* depthStencilBuffer = nullptr;
	ID3D12DescriptorHeap* depthStencilDescHeap = nullptr;
	UINT depthStencilIncrementSize = 0;

	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;

	ID3D12PipelineState* pipeLineState = nullptr;

	ID3D12Fence* fence = nullptr;
	UINT64 fenceValues[swapChainBuffers] = { 0, 0 };
	HANDLE fenceEvent = nullptr;

#if defined(_DEBUG)
	ID3D12Debug* debugController = nullptr;
#endif


	Scene* scene = nullptr;
	Camera* camera = nullptr;
public:

	bool OnInit(HINSTANCE hInstance, HWND hWnd);
	void OnDestroy();

	void CreateDirect3DDevice();
	void CreateCommandQueueAndList();
	void CreateSwapChain();
	void CreateRtvAndDsvDescriptorHeaps();
	void CreateRenderTargetView();
	void CreateDepthStencilView();

	void BuildObjects();
	void ReleaseObjects();

	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();

	void WaitForGpuComplete();
	void MoveToNextFrame();
	void ChangeSwapChainState();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

