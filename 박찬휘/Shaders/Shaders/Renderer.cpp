#include "framework.h"
#include "Renderer.h"
#include "Device.h"

Renderer::Renderer(HINSTANCE hInstance, HWND hWnd)
	:hInstance(hInstance), hWnd(hWnd)
{
	OnInit();
}

bool Renderer::OnInit()
{
	device = new Device();

	CreateSwpaChain();
	CreateRtvAndDsvDescHeap();
	CreateDepthStencilView();
	InitMultiSampleQualityLevels();

	return true;
}

void Renderer::OnDestroy()
{
	if (device)
	{
		device->OnDestroy();
		delete device;
		device = nullptr;
	}
}

void Renderer::CreateSwpaChain()
{
	RECT rcClient;
	::GetClientRect(hWnd, &rcClient);
	clientWidth = rcClient.right - rcClient.left;
	clientHeight = rcClient.bottom - rcClient.top;

#ifdef _WITH_CREATE_SWAPCHAIN_FOR_HWND
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
	::ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC1));
	swapChainDesc.Width = clientWidth;
	swapChainDesc.Height = clientHeight;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = (Msaa4xEnable) ? 4 : 1;
	swapChainDesc.SampleDesc.Quality = (Msaa4xEnable) ? (Msaa4xQualityLevels - 1) : 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = swapChainBuffers;
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC dxgiSwapChainFullScreenDesc;
	::ZeroMemory(&dxgiSwapChainFullScreenDesc, sizeof(DXGI_SWAP_CHAIN_FULLSCREEN_DESC));
	dxgiSwapChainFullScreenDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainFullScreenDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainFullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc.Windowed = TRUE;

	HR(device->GetDXGIFactory()->CreateSwapChainForHwnd(m_pd3dCommandQueue, hWnd, &swapChainDesc, &dxgiSwapChainFullScreenDesc, NULL, (IDXGISwapChain1**)&m_pdxgiSwapChain));
#else
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	::ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = swapChainBuffers;
	swapChainDesc.BufferDesc.Width = clientWidth;
	swapChainDesc.BufferDesc.Height = clientHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = (Msaa4xEnable) ? 4 : 1;
	swapChainDesc.SampleDesc.Quality = (Msaa4xEnable) ? (Msaa4xQualityLevels - 1) : 0;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HR(device->GetDXGIFactory()->CreateSwapChain(device->GetCommandQueue(), &swapChainDesc, (IDXGISwapChain**)&swapChain));
#endif
	swapChainBufferIndex = swapChain->GetCurrentBackBufferIndex();

	HR(device->GetDXGIFactory()->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

#ifndef _WITH_SWAPCHAIN_FULLSCREEN_STATE
	//CreateRenderTargetViews();
#endif
}

void Renderer::CreateRtvAndDsvDescHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc;
	::ZeroMemory(&descriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	descriptorHeapDesc.NumDescriptors = swapChainBuffers;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	descriptorHeapDesc.NodeMask = 0;
	HR(this->device->GetDX12Device()->CreateDescriptorHeap(&descriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&renderTargetDescHeap));

	descriptorHeapDesc.NumDescriptors = 1;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	HR(this->device->GetDX12Device()->CreateDescriptorHeap(&descriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&depthStencilDescHeap));

}

void Renderer::CreateRenderTargetViews()
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvCPUDescriptorHandle = renderTargetDescHeap->GetCPUDescriptorHandleForHeapStart();
	for (UINT i = 0; i < swapChainBuffers; i++)
	{
		HR(swapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)&swapChainBackBuffers[i]));
		this->device->GetDX12Device()->CreateRenderTargetView(swapChainBackBuffers[i], NULL, rtvCPUDescriptorHandle);
		rtvCPUDescriptorHandle.ptr += gResourceHelper.GetRtvDescriptorIncrementSize();
	}
}

void Renderer::CreateDepthStencilView()
{
	D3D12_RESOURCE_DESC resourceDesc;
	::ZeroMemory(&resourceDesc, sizeof(D3D12_RESOURCE_DESC));
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = clientWidth;
	resourceDesc.Height = clientHeight;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	resourceDesc.SampleDesc.Count = (Msaa4xEnable) ? 4 : 1;
	resourceDesc.SampleDesc.Quality = (Msaa4xEnable) ? (Msaa4xQualityLevels - 1) : 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_HEAP_PROPERTIES heapProperties;
	::ZeroMemory(&heapProperties, sizeof(D3D12_HEAP_PROPERTIES));
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;

	D3D12_CLEAR_VALUE clearValue;
	::ZeroMemory(&clearValue, sizeof(D3D12_CLEAR_VALUE));
	clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	HR(this->device->GetDX12Device()->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue, __uuidof(ID3D12Resource), (void**)&depthStencilBuffer));

	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	::ZeroMemory(&depthStencilViewDesc, sizeof(D3D12_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

	D3D12_CPU_DESCRIPTOR_HANDLE dsvCPUDescriptorHandle = depthStencilDescHeap->GetCPUDescriptorHandleForHeapStart();
	this->device->GetDX12Device()->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, dsvCPUDescriptorHandle);

}

void Renderer::InitMultiSampleQualityLevels()
{
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS MsaaQualityLevels;
	MsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	MsaaQualityLevels.SampleCount = 4;
	MsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	MsaaQualityLevels.NumQualityLevels = 0;
	HR(this->device->GetDX12Device()->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&MsaaQualityLevels,
		sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS)
	));

	this->Msaa4xQualityLevels = MsaaQualityLevels.NumQualityLevels;
	this->Msaa4xEnable = (Msaa4xQualityLevels > 1) ? true : false;
}

void Renderer::Render()
{
	HR(this->device->GetCommandAllocator()->Reset());
	HRESULT t = this->device->GetCommandList()->Reset(this->device->GetCommandAllocator(), nullptr);
	D3D12_RESOURCE_BARRIER resourceBarrier;
	::ZeroMemory(&resourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
	resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resourceBarrier.Transition.pResource = swapChainBackBuffers[swapChainBufferIndex];
	resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	this->device->GetCommandList()->ResourceBarrier(1, &resourceBarrier);

	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = renderTargetDescHeap->GetCPUDescriptorHandleForHeapStart();
	d3dRtvCPUDescriptorHandle.ptr += (swapChainBufferIndex * gResourceHelper.GetRtvDescriptorIncrementSize());

	this->device->GetCommandList()->ClearRenderTargetView(d3dRtvCPUDescriptorHandle, Colors::Black, 0, NULL);

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = depthStencilDescHeap->GetCPUDescriptorHandleForHeapStart();
	this->device->GetCommandList()->ClearDepthStencilView(d3dDsvCPUDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

	this->device->GetCommandList()->OMSetRenderTargets(1, &d3dRtvCPUDescriptorHandle, TRUE, &d3dDsvCPUDescriptorHandle);


	//TODO : Render Scene

	resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	this->device->GetCommandList()->ResourceBarrier(1, &resourceBarrier);

	HRESULT h = this->device->GetCommandList()->Close();

	ID3D12CommandList* ppd3dCommandLists[] = { this->device->GetCommandList() };
	this->device->GetCommandQueue()->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGPUComplete();

	swapChain->Present(0, 0);

	MoveToNextFrame();

}

void Renderer::MoveToNextFrame()
{
	swapChainBufferIndex = swapChain->GetCurrentBackBufferIndex();
	const UINT64 nFenceValue = device->GetFenceValue(swapChainBufferIndex) + 1;
	HRESULT hResult = device->GetCommandQueue()->Signal(device->GetFence(), nFenceValue);

	if (device->GetFence()->GetCompletedValue() < nFenceValue)
	{
		hResult = device->GetFence()->SetEventOnCompletion(nFenceValue, device->GetFenceEvent());
		::WaitForSingleObject(device->GetFenceEvent(), INFINITE);
	}
}

void Renderer::WaitForGPUComplete()
{
	const UINT64 nFenceValue = device->GetFenceValue(swapChainBufferIndex) + 1;
	HRESULT hResult = device->GetCommandQueue()->Signal(device->GetFence(), nFenceValue);

	if (device->GetFence()->GetCompletedValue() < nFenceValue)
	{
		hResult = device->GetFence()->SetEventOnCompletion(nFenceValue, device->GetFenceEvent());
		::WaitForSingleObject(device->GetFenceEvent(), INFINITE);
	}
}
