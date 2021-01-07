#pragma once

class Device;

class Renderer
{
public:

	Renderer() = default;
	Renderer(HINSTANCE hInstance, HWND hWNd);
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer() = default;

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

	Device* device = nullptr;


private:

	bool OnInit();

	void CreateSwapChain();
	void CreateRtvAndDsvDescHeap();
	void CreateRenderTargetViews();
	void CreateDepthStencilView();
	void InitMultiSampleQualityLevels();

public:

	void OnDestroy();

	void Render();
	void MoveToNextFrame();
	void WaitForGPUComplete();
};

