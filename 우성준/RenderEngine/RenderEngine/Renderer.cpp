#include "framework.h"
#include "Renderer.h"
#include "Device.h"

Renderer::Renderer(ID3D12CommandAllocator* commandAllocator, ID3D12CommandQueue* commandQueue, ID3D12GraphicsCommandList* commandList)
	:commandAllocator(commandAllocator), commandQueue(commandQueue),commandList(commandList)
{

}

Renderer::~Renderer()
{
}

void Renderer::OnInit(HINSTANCE hInstance, HWND hWnd, ID3D12Device* device)
{
	this->hInstance = hInstance;
	this->hWnd = hWnd;

	CreateRtvAndDsvDescHeap(device);
	CreateSwpaChain();
	CreateDepthStencilView(device);
}


void Renderer::OnDestroy()
{
}

void Renderer::CreateSwpaChain()
{
	IDXGIFactory4* factory4 = nullptr;
	UINT nDXGIFactoryFlags = 0;
	HR(::CreateDXGIFactory2(nDXGIFactoryFlags, __uuidof(IDXGIFactory4), (void**)&factory4));
	
	IDXGIAdapter1* pd3dAdapter = NULL;

	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != factory4->EnumAdapters1(i, &pd3dAdapter); i++)
	{
		DXGI_ADAPTER_DESC1 dxgiAdapterDesc;
		HR(pd3dAdapter->GetDesc1(&dxgiAdapterDesc));
		if (dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;
	}

	RECT rcClient;
	::GetClientRect(hWnd, &rcClient);
	clientWidth = rcClient.right - rcClient.left;
	clientHeight = rcClient.bottom - rcClient.top;

#ifdef _WITH_CREATE_SWAPCHAIN_FOR_HWND
	DXGI_SWAP_CHAIN_DESC1 dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC1));
	dxgiSwapChainDesc.Width = clientWidth;
	dxgiSwapChainDesc.Height = clientHeight;
	dxgiSwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.SampleDesc.Count = (Msaa4xEnable) ? 4 : 1;
	dxgiSwapChainDesc.SampleDesc.Quality = (Msaa4xEnable) ? (Msaa4xQualityLevels - 1) : 0;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.BufferCount = swapChainBuffers;
	dxgiSwapChainDesc.Scaling = DXGI_SCALING_NONE;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC dxgiSwapChainFullScreenDesc;
	::ZeroMemory(&dxgiSwapChainFullScreenDesc, sizeof(DXGI_SWAP_CHAIN_FULLSCREEN_DESC));
	dxgiSwapChainFullScreenDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainFullScreenDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainFullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc.Windowed = TRUE;

	HRESULT hResult = m_pdxgiFactory->CreateSwapChainForHwnd(commandQueue, m_hWnd, &dxgiSwapChainDesc, &dxgiSwapChainFullScreenDesc, NULL, (IDXGISwapChain1**)&m_pdxgiSwapChain);
#else
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = swapChainBuffers;
	dxgiSwapChainDesc.BufferDesc.Width = clientWidth;
	dxgiSwapChainDesc.BufferDesc.Height = clientHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.OutputWindow = hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = (Msaa4xEnable) ? 4 : 1;
	dxgiSwapChainDesc.SampleDesc.Quality = (Msaa4xEnable) ? (Msaa4xQualityLevels - 1) : 0;
	dxgiSwapChainDesc.Windowed = TRUE;
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT h = factory4->CreateSwapChain(commandQueue, &dxgiSwapChainDesc, (IDXGISwapChain**)&swapChain);
#endif
	swapChainBufferIndex = swapChain->GetCurrentBackBufferIndex();

	HR(factory4->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

#ifndef _WITH_SWAPCHAIN_FULLSCREEN_STATE
	//CreateRenderTargetViews();
#endif
}

void Renderer::CreateRtvAndDsvDescHeap(ID3D12Device* device)
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	d3dDescriptorHeapDesc.NumDescriptors = swapChainBuffers;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	HR(device->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&renderTargetDescHeap));

	d3dDescriptorHeapDesc.NumDescriptors = 1;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	HR(device->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&depthStencilDescHeap));

}

void Renderer::CreateRenderTargetView(ID3D12Device* device)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvCPUDescriptorHandle = renderTargetDescHeap->GetCPUDescriptorHandleForHeapStart();
	for (UINT i = 0; i < swapChainBuffers; i++)
	{
		HR(swapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)&swapChainBackBuffers[i]));
		device->CreateRenderTargetView(swapChainBackBuffers[i], NULL, rtvCPUDescriptorHandle);
		rtvCPUDescriptorHandle.ptr += resourceHelper.GetRtvDescriptorIncrementSize();
	}
}

void Renderer::CreateDepthStencilView(ID3D12Device* device)
{
	D3D12_RESOURCE_DESC d3dResourceDesc;
	d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	d3dResourceDesc.Alignment = 0;
	d3dResourceDesc.Width = clientWidth;
	d3dResourceDesc.Height = clientHeight;
	d3dResourceDesc.DepthOrArraySize = 1;
	d3dResourceDesc.MipLevels = 1;
	d3dResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dResourceDesc.SampleDesc.Count = (Msaa4xEnable) ? 4 : 1;
	d3dResourceDesc.SampleDesc.Quality = (Msaa4xEnable) ? (Msaa4xQualityLevels - 1) : 0;
	d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_HEAP_PROPERTIES d3dHeapProperties;
	::ZeroMemory(&d3dHeapProperties, sizeof(D3D12_HEAP_PROPERTIES));
	d3dHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	d3dHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	d3dHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	d3dHeapProperties.CreationNodeMask = 1;
	d3dHeapProperties.VisibleNodeMask = 1;

	D3D12_CLEAR_VALUE d3dClearValue;
	d3dClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dClearValue.DepthStencil.Depth = 1.0f;
	d3dClearValue.DepthStencil.Stencil = 0;

	HR(device->CreateCommittedResource(&d3dHeapProperties, D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &d3dClearValue, __uuidof(ID3D12Resource), (void**)&depthStencilBuffer));

	D3D12_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	::ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D12_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dDepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = depthStencilDescHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateDepthStencilView(depthStencilBuffer, &d3dDepthStencilViewDesc, d3dDsvCPUDescriptorHandle);

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
	HR(commandAllocator->Reset());
	HRESULT t = commandList->Reset(commandAllocator, nullptr);
	D3D12_RESOURCE_BARRIER d3dResourceBarrier;

	::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
	d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier.Transition.pResource = swapChainBackBuffers[swapChainBufferIndex];
	d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	commandList->ResourceBarrier(1, &d3dResourceBarrier);

	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = renderTargetDescHeap->GetCPUDescriptorHandleForHeapStart();
	d3dRtvCPUDescriptorHandle.ptr += (swapChainBufferIndex * resourceHelper.GetRtvDescriptorIncrementSize());

	commandList->ClearRenderTargetView(d3dRtvCPUDescriptorHandle, Colors::Black, 0, NULL);

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = depthStencilDescHeap->GetCPUDescriptorHandleForHeapStart();
	commandList->ClearDepthStencilView(d3dDsvCPUDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

	commandList->OMSetRenderTargets(1, &d3dRtvCPUDescriptorHandle, TRUE, &d3dDsvCPUDescriptorHandle);

	d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	commandList->ResourceBarrier(1, &d3dResourceBarrier);

	HRESULT h = commandList->Close();

	ID3D12CommandList* ppd3dCommandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	swapChain->Present(0, 0);
}

void Renderer::WaitForGPUComlete()
{
}

ID3D12Resource** Renderer::GetSwapChainBackBuffers()
{
	return this->swapChainBackBuffers;
}

ID3D12DescriptorHeap* Renderer::GetRenderTargetDescHeap()
{
	return this->renderTargetDescHeap;
}
