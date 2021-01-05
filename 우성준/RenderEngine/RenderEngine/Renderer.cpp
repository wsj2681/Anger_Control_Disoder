#include "framework.h"
#include "Renderer.h"

Renderer::Renderer(ID3D12CommandAllocator* commandAllocator, ID3D12CommandQueue* commandQueue, ID3D12GraphicsCommandList* commandList)
	:commandAllocator(commandAllocator), commandQueue(commandQueue),commandList(commandList)
{

}

Renderer::~Renderer()
{
}

void Renderer::OnInit(HINSTANCE hInstance, HWND hWnd)
{
	this->hInstance = hInstance;
	this->hWnd = hWnd;

	CreateSwpaChain();
}

void Renderer::OnDestroy()
{
}

void Renderer::CreateSwpaChain()
{

}

void Renderer::InitMultiSampleQualityLevels(ID3D12Device* device)
{
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS MsaaQualityLevels;
	MsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	MsaaQualityLevels.SampleCount = 4;
	MsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	MsaaQualityLevels.NumQualityLevels = 0;
	device->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, 
		&MsaaQualityLevels, 
		sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS)
	);

	this->Msaa4xQualityLevels = MsaaQualityLevels.NumQualityLevels;
	this->Msaa4xEnable = (Msaa4xQualityLevels > 1) ? true : false;
}

void Renderer::Render()
{
	commandAllocator->Reset();
	commandList->Reset(commandAllocator, nullptr);


}

void Renderer::SetRenderTargetDescIncrementSize(UINT size)
{
	this->renderTargetDescIncremetSize = size;
}

UINT Renderer::GetRenderTargetDescIncrementSize()
{
	return this->renderTargetDescIncremetSize;
}

ID3D12Resource** Renderer::GetSwapChainBackBuffers()
{
	return this->swapChainBackBuffers;
}

ID3D12DescriptorHeap* Renderer::GetRenderTargetDescHeap()
{
	return this->renderTargetDescHeap;
}
