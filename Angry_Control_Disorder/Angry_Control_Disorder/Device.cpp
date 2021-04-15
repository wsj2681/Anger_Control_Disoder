#include "framework.h"
#include "Device.h"

bool Device::Init(HINSTANCE hInstance, HWND hWnd)
{
	this->hInstance = hInstance;
	this->hWnd = hWnd;

	CreateDirect3DDevice();
	CreateCommandQueueAndList();
	CreateRtvAndDsvDescriptorHeaps();
	CreateSwapChain();
	CreateDepthStencilView();

	HR(CoInitialize(nullptr));

	BuildObjects();

	return true;
}

void Device::Release()
{
	ReleaseObjects();

	::CloseHandle(fenceEvent);

	RELEASE(depthStencilBuffer);
	RELEASE(dsvDescHeap);
	for (int i = 0; i < swapChainBuffers; i++) if (swapChainBackBuffers[i]) swapChainBackBuffers[i]->Release();
	RELEASE(rtvDescHeap);
	RELEASE(commandAllocator);
	RELEASE(commandQueue);
	RELEASE(commandList);
	RELEASE(fence);

	swapChain->SetFullscreenState(false, nullptr);
	RELEASE(swapChain);
	RELEASE(device);
	RELEASE(factory);

#if defined(_DEBUG)
	IDXGIDebug1* pdxgiDebug = NULL;
	DXGIGetDebugInterface1(0, __uuidof(IDXGIDebug1), (void**)&pdxgiDebug);
	HRESULT hResult = pdxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
	pdxgiDebug->Release();
#endif

	delete this;
}

void Device::CreateSwapChain()
{
	RECT rcClient;
	::GetClientRect(this->hWnd, &rcClient);
	wndClientWidth = rcClient.right - rcClient.left;
	wndClientHeight = rcClient.bottom - rcClient.top;

	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = swapChainBuffers;
	dxgiSwapChainDesc.BufferDesc.Width = wndClientWidth;
	dxgiSwapChainDesc.BufferDesc.Height = wndClientHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.OutputWindow = this->hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = (bMsaa4xEnable) ? 4 : 1;
	dxgiSwapChainDesc.SampleDesc.Quality = (bMsaa4xEnable) ? (nMsaa4xQualityLevels - 1) : 0;
	dxgiSwapChainDesc.Windowed = TRUE;
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HR(factory->CreateSwapChain(commandQueue, &dxgiSwapChainDesc, (IDXGISwapChain**)&swapChain));

	swapChainBufferIndex = swapChain->GetCurrentBackBufferIndex();

	HR(factory->MakeWindowAssociation(this->hWnd, DXGI_MWA_NO_ALT_ENTER));

#ifndef _WITH_SWAPCHAIN_FULLSCREEN_STATE
	CreateRenderTargetViews();
#endif
}

void Device::CreateDirect3DDevice()
{
	UINT nDXGIFactoryFlags = 0;
#if defined(_DEBUG)
	ID3D12Debug* pd3dDebugController = nullptr;
	HR(D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&pd3dDebugController));
	if (pd3dDebugController)
	{
		pd3dDebugController->EnableDebugLayer();
		pd3dDebugController->Release();
	}
	nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	HR(::CreateDXGIFactory2(nDXGIFactoryFlags, __uuidof(IDXGIFactory4), (void**)&factory));

	IDXGIAdapter1* pd3dAdapter = nullptr;

	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &pd3dAdapter); i++)
	{
		DXGI_ADAPTER_DESC1 dxgiAdapterDesc;
		pd3dAdapter->GetDesc1(&dxgiAdapterDesc);
		if (dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}
		if (SUCCEEDED(D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void**)&device)))
		{
			break;
		}
	}

	if (!pd3dAdapter)
	{
		factory->EnumWarpAdapter(_uuidof(IDXGIFactory4), (void**)&pd3dAdapter);
		HR(D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void**)&device));
	}

	if (!device)
	{
		factory->EnumWarpAdapter(_uuidof(IDXGIFactory4), (void**)&pd3dAdapter);
		HR(D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_11_1, _uuidof(ID3D12Device), (void**)&device));
	}

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMsaaQualityLevels;
	d3dMsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dMsaaQualityLevels.SampleCount = 4;
	d3dMsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	d3dMsaaQualityLevels.NumQualityLevels = 0;
	HR(device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &d3dMsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS)));
	nMsaa4xQualityLevels = d3dMsaaQualityLevels.NumQualityLevels;
	bMsaa4xEnable = (nMsaa4xQualityLevels > 1) ? true : false;

	HR(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&fence));
	for (UINT i = 0; i < swapChainBuffers; i++)
	{
		fenceValues[i] = 0;
	}

	fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	RELEASE(pd3dAdapter);
}

void Device::CreateCommandQueueAndList()
{
	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc;
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	d3dCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3dCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HR(device->CreateCommandQueue(&d3dCommandQueueDesc, _uuidof(ID3D12CommandQueue), (void**)&commandQueue));

	HR(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&commandAllocator));

	HR(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&commandList));
	HR(commandList->Close());
}

void Device::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	d3dDescriptorHeapDesc.NumDescriptors = swapChainBuffers;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	HR(device->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&rtvDescHeap));
	rtvDescIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	d3dDescriptorHeapDesc.NumDescriptors = 1;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	HR(device->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&dsvDescHeap));
	dsvDescIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

void Device::CreateRenderTargetViews()
{
	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = rtvDescHeap->GetCPUDescriptorHandleForHeapStart();
	for (UINT i = 0; i < swapChainBuffers; i++)
	{
		swapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)&swapChainBackBuffers[i]);
		device->CreateRenderTargetView(swapChainBackBuffers[i], NULL, d3dRtvCPUDescriptorHandle);
		d3dRtvCPUDescriptorHandle.ptr += rtvDescIncrementSize;
	}
}

void Device::CreateDepthStencilView()
{
	D3D12_RESOURCE_DESC d3dResourceDesc;
	d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	d3dResourceDesc.Alignment = 0;
	d3dResourceDesc.Width = wndClientWidth;
	d3dResourceDesc.Height = wndClientHeight;
	d3dResourceDesc.DepthOrArraySize = 1;
	d3dResourceDesc.MipLevels = 1;
	d3dResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dResourceDesc.SampleDesc.Count = (bMsaa4xEnable) ? 4 : 1;
	d3dResourceDesc.SampleDesc.Quality = (bMsaa4xEnable) ? (nMsaa4xQualityLevels - 1) : 0;
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

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = dsvDescHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateDepthStencilView(depthStencilBuffer, &d3dDepthStencilViewDesc, d3dDsvCPUDescriptorHandle);
}

void Device::ChangeSwapChainState()
{
	WaitForGpuComplete();

	BOOL bFullScreenState = false;
	HR(swapChain->GetFullscreenState(&bFullScreenState, nullptr));
	HR(swapChain->SetFullscreenState(!bFullScreenState, nullptr));

	DXGI_MODE_DESC dxgiTargetParameters;
	dxgiTargetParameters.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiTargetParameters.Width = wndClientWidth;
	dxgiTargetParameters.Height = wndClientHeight;
	dxgiTargetParameters.RefreshRate.Numerator = 60;
	dxgiTargetParameters.RefreshRate.Denominator = 1;
	dxgiTargetParameters.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgiTargetParameters.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	HR(swapChain->ResizeTarget(&dxgiTargetParameters));

	for (int i = 0; i < swapChainBuffers; i++) if (swapChainBackBuffers[i]) swapChainBackBuffers[i]->Release();

	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	HR(swapChain->GetDesc(&dxgiSwapChainDesc));
	HR(swapChain->ResizeBuffers(swapChainBuffers, wndClientWidth, wndClientHeight, dxgiSwapChainDesc.BufferDesc.Format, dxgiSwapChainDesc.Flags));

	swapChainBufferIndex = swapChain->GetCurrentBackBufferIndex();

	CreateRenderTargetViews();
}

void Device::BuildObjects()
{
}

void Device::ReleaseObjects()
{
	
}

void Device::ProcessInput()
{
}

void Device::AnimateObjects()
{
	//float fTimeElapsed = m_GameTimer.GetTimeElapsed();
}

bool Device::FrameAdvance()
{
	ProcessInput();

	AnimateObjects();

	HR(commandAllocator->Reset());
	HR(commandList->Reset(commandAllocator, nullptr));

	D3D12_RESOURCE_BARRIER d3dResourceBarrier;
	::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
	d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier.Transition.pResource = swapChainBackBuffers[swapChainBufferIndex];
	d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	commandList->ResourceBarrier(1, &d3dResourceBarrier);

	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = rtvDescHeap->GetCPUDescriptorHandleForHeapStart();
	d3dRtvCPUDescriptorHandle.ptr += (swapChainBufferIndex * rtvDescIncrementSize);

	float pfClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	commandList->ClearRenderTargetView(d3dRtvCPUDescriptorHandle, pfClearColor/*Colors::Azure*/, 0, NULL);

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = dsvDescHeap->GetCPUDescriptorHandleForHeapStart();
	commandList->ClearDepthStencilView(d3dDsvCPUDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

	commandList->OMSetRenderTargets(1, &d3dRtvCPUDescriptorHandle, TRUE, &d3dDsvCPUDescriptorHandle);

	//if (m_pScene) m_pScene->Render(commandList, m_pCamera);

#ifdef _WITH_PLAYER_TOP
	commandList->ClearDepthStencilView(d3dDsvCPUDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
#endif
	//if (m_pPlayer) m_pPlayer->Render(commandList, m_pCamera);

	d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	commandList->ResourceBarrier(1, &d3dResourceBarrier);

	HR(commandList->Close());

	ID3D12CommandList* ppd3dCommandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete();

	swapChain->Present(0, 0);

	MoveToNextFrame();

	//m_GameTimer.GetFrameRate(frameRate + 12, 37);
	//size_t nLength = _tcslen(frameRate);
	//XMFLOAT3 xmf3Position = m_pPlayer->GetPosition();
	//_stprintf_s(frameRate + nLength, 70 - nLength, _T("(%4f, %4f, %4f)"), xmf3Position.x, xmf3Position.y, xmf3Position.z);
	//::SetWindowText(this->hWnd, frameRate);

	return true;
}

void Device::WaitForGpuComplete()
{
	const UINT64 nFenceValue = ++fenceValues[swapChainBufferIndex];
	HR(commandQueue->Signal(fence, nFenceValue));

	if (fence->GetCompletedValue() < nFenceValue)
	{
		HR(fence->SetEventOnCompletion(nFenceValue, fenceEvent));
		::WaitForSingleObject(fenceEvent, INFINITE);
	}
}

void Device::MoveToNextFrame()
{
	swapChainBufferIndex = swapChain->GetCurrentBackBufferIndex();
	//swapChainBufferIndex = (swapChainBufferIndex + 1) % swapChainBuffers;

	UINT64 nFenceValue = ++fenceValues[swapChainBufferIndex];
	HR(commandQueue->Signal(fence, nFenceValue));

	if (fence->GetCompletedValue() < nFenceValue)
	{
		HR(fence->SetEventOnCompletion(nFenceValue, fenceEvent));
		::WaitForSingleObject(fenceEvent, INFINITE);
	}
}

void Device::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void Device::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void Device::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}
