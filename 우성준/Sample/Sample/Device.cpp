#include "framework.h"
#include "Device.h"

Device::Device()
{
	OnInit();
}

bool Device::OnInit()
{
	this->CreateDevice();
	this->CreateCommander();

	return true;
}

void Device::OnDestroy()
{
	::CloseHandle(fenceEvent);
	SAFE_RELEASE(commandAllocator);
	SAFE_RELEASE(commandQueue);
	SAFE_RELEASE(commandList);
	SAFE_RELEASE(fence);
	SAFE_RELEASE(device);
	SAFE_RELEASE(factory4);

#if defined(_DEBUG)
	SAFE_RELEASE(debugController);
#endif
}

void Device::CreateDevice()
{

	UINT factoryFlags = 0;

#if defined(_DEBUG)
	HR(D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&this->debugController));
	if (debugController)
	{
		debugController->EnableDebugLayer();
	}
	factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	HR(::CreateDXGIFactory2(factoryFlags, __uuidof(IDXGIFactory4), (void**)&factory4));

	IDXGIAdapter1* adapter = nullptr;

	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != factory4->EnumAdapters1(i, &adapter); i++)
	{
		DXGI_ADAPTER_DESC1 dxgiAdapterDesc;
		HR(adapter->GetDesc1(&dxgiAdapterDesc));
		if (dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;
		if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void**)&device))) break;
	}

	if (!adapter)
	{
		HR(factory4->EnumWarpAdapter(_uuidof(IDXGIFactory4), (void**)&adapter));
		HR(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void**)&device));
	}

	gResourceHelper.SetCbvSrvDescriptorIncrementSize(device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	gResourceHelper.SetRtvDescriptorIncrementSize(device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	gResourceHelper.SetDsvDescriptorIncrementSize(device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV));

	HR(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&fence));
	for (UINT i = 0; i < 2; ++i)
		fenceValues[i] = 0;

	fenceEvent = ::CreateEvent(nullptr, false, false, nullptr);

	if (adapter) adapter->Release();

}

void Device::CreateCommander()
{
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
	::ZeroMemory(&commandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HR(device->CreateCommandQueue(&commandQueueDesc, _uuidof(ID3D12CommandQueue), (void**)&commandQueue));
	HR(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&commandAllocator));
	HR(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, __uuidof(ID3D12GraphicsCommandList), (void**)&commandList));
	HR(commandList->Close());
}

ID3D12Device*& Device::GetDX12Device()
{
	return this->device;
}

IDXGIFactory4*& Device::GetDXGIFactory()
{
	return this->factory4;
}

ID3D12CommandAllocator*& Device::GetCommandAllocator()
{
	return this->commandAllocator;
}

ID3D12CommandQueue*& Device::GetCommandQueue()
{
	return this->commandQueue;
}

ID3D12GraphicsCommandList*& Device::GetCommandList()
{
	return this->commandList;
}

ID3D12Fence*& Device::GetFence()
{
	return this->fence;
}

UINT64& Device::GetFenceValue(int index)
{
	return this->fenceValues[index];
}

HANDLE& Device::GetFenceEvent()
{
	return this->fenceEvent;
}
