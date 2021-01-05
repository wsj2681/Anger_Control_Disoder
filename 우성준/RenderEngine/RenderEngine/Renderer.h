#pragma once
class Renderer final
{
public:
	Renderer() = default;
	Renderer(ID3D12CommandAllocator* commandAllocator,
			ID3D12CommandQueue* commandQueue, 
			ID3D12GraphicsCommandList* commandList);
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer();

private:

	HINSTANCE hInstance = nullptr;
	HWND hWnd = nullptr;

	int clientWidth = 0;
	int clientHeight = 0;

	IDXGISwapChain3* swapChain = nullptr;
	static const UINT swapChainBuffers = 2;
	UINT swapChainBufferIndex = 0;

	ID3D12Resource* swapChainBackBuffers[swapChainBuffers]{ nullptr, nullptr };
	ID3D12DescriptorHeap* renderTargetDescHeap = nullptr;

	ID3D12DescriptorHeap* depthStencilDescHeap = nullptr;
	ID3D12Resource* depthStencilBuffer = nullptr;

	bool Msaa4xEnable = false;
	UINT Msaa4xQualityLevels = 0;

	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;



public:

	void OnInit(HINSTANCE hInstance, HWND hWnd, ID3D12Device* device);
	void OnDestroy();

	void CreateSwpaChain();
	void CreateRtvAndDsvDescHeap();
	void CreateRenderTargetView();
	void CreateDepthStencilView();

	void InitMultiSampleQualityLevels(ID3D12Device* device);
	
	void Render();

	ID3D12Resource** GetSwapChainBackBuffers();
	ID3D12DescriptorHeap* GetRenderTargetDescHeap();

};

