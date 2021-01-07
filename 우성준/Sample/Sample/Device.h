#pragma once
class Device final
{
public:
	
	Device();
	Device(const Device&) = delete;
	Device& operator=(const Device&) = delete;
	~Device() = default;

private:

	ID3D12Device* device = nullptr;
	IDXGIFactory4* factory4 = nullptr;

	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;

	ID3D12Fence* fence = nullptr;
	UINT64 fenceValues[2]{ 0, 0 };
	HANDLE fenceEvent = nullptr;

#if defined(_DEBUG)
	ID3D12Debug* debugController = nullptr;
#endif

private:
	
	bool OnInit();
	void CreateDevice();
	void CreateCommander();

public:

	void OnDestroy();
	
public:/* Get */

	ID3D12Device* GetDX12Device();
	IDXGIFactory4* GetDXGIFactory();

	ID3D12CommandAllocator* GetCommandAllocator();
	ID3D12CommandQueue* GetCommandQueue();
	ID3D12GraphicsCommandList* GetCommandList();

	ID3D12Fence* GetFence();
	UINT64 GetFenceValue(int index);
	HANDLE GetFenceEvent();

};

