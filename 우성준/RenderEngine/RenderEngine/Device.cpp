#include "framework.h"
#include "Device.h"

void Device::OnInit()
{
}

void Device::OnDestroy()
{
}

void Device::CreateDevice()
{
}

void Device::CreateCommander()
{
}

ID3D12CommandAllocator* Device::GetCommandAllocator()
{
	return this->commandAllocator;
}

ID3D12CommandQueue* Device::GetCommandQueue()
{
	return this->commandQueue;
}

ID3D12GraphicsCommandList* Device::GetCommandList()
{
	return this->commandList;
}

Device::Device()
{
}

Device::~Device()
{
}
