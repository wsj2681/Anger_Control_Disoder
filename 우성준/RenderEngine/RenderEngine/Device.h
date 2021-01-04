#pragma once
class Device final
{
public:
	Device();
	Device(const Device&) = delete;
	Device& operator=(const Device&) = delete;
	~Device();

private:

	IDXGIFactory4* factory4 = nullptr;
	ID3D12Device* device = nullptr;

	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12CommandList* commandList = nullptr;

	ID3D12Fence* fence = nullptr;
	UINT64 fenceValues[2];
	HANDLE fenceEvent = nullptr;

public:
	void OnInit();
	void OnDestroy();

	void CreateDevice();
	void CreateCommander();

};

