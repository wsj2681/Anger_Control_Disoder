#include "framework.h"
#include "Engine.h"

bool Engine::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	Instance = hInstance;
	Wnd = hMainWnd;

	CreateDirect3DDevice();
	CreateCommandQueueAndList();
	CreateRtvAndDsvDescriptorHeaps();
	CreateSwapChain();
	CreateDepthStencilView();

	BuildObjects();

	return(true);
}

void Engine::CreateSwapChain()
{
	RECT rcClient;
	::GetClientRect(Wnd, &rcClient);
	WndClientWidth = rcClient.right - rcClient.left;
	WndClientHeight = rcClient.bottom - rcClient.top;

#ifdef _WITH_CREATE_SWAPCHAIN_FOR_HWND
	DXGI_SWAP_CHAIN_DESC1 dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC1));
	dxgiSwapChainDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
	dxgiSwapChainDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.BufferCount = m_nSwapChainBuffers;
	dxgiSwapChainDesc.Scaling = DXGI_SCALING_NONE;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
#ifdef _WITH_ONLY_RESIZE_BACKBUFFERS
	dxgiSwapChainDesc.Flags = 0;
#else
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#endif

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC dxgiSwapChainFullScreenDesc;
	::ZeroMemory(&dxgiSwapChainFullScreenDesc, sizeof(DXGI_SWAP_CHAIN_FULLSCREEN_DESC));
	dxgiSwapChainFullScreenDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainFullScreenDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainFullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc.Windowed = TRUE;

	HR(m_pdxgiFactory->CreateSwapChainForHwnd(m_pd3dCommandQueue, m_hWnd, &dxgiSwapChainDesc, &dxgiSwapChainFullScreenDesc, NULL, (IDXGISwapChain1**)&m_pdxgiSwapChain));
#else
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = SwapChainBuffers;
	dxgiSwapChainDesc.BufferDesc.Width = WndClientWidth;
	dxgiSwapChainDesc.BufferDesc.Height = WndClientHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.OutputWindow = Wnd;
	dxgiSwapChainDesc.SampleDesc.Count = (Msaa4xEnable) ? 4 : 1;
	dxgiSwapChainDesc.SampleDesc.Quality = (Msaa4xEnable) ? (Msaa4xQualityLevels - 1) : 0;
	dxgiSwapChainDesc.Windowed = TRUE;

#ifdef _WITH_ONLY_RESIZE_BACKBUFFERS
	dxgiSwapChainDesc.Flags = 0;
#else
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#endif

	HR(Factory->CreateSwapChain(CommandQueue, &dxgiSwapChainDesc, (IDXGISwapChain**)&SwapChain));
#endif

	HR(Factory->MakeWindowAssociation(Wnd, DXGI_MWA_NO_ALT_ENTER));
	SwapChainBufferIndex = SwapChain->GetCurrentBackBufferIndex();

#ifndef _WITH_SWAPCHAIN_FULLSCREEN_STATE
	CreateRenderTargetViews();
#endif
}

void Engine::CreateDirect3DDevice()
{
	UINT nDXGIFactoryFlags = 0;


#if defined(_DEBUG)

	ID3D12Debug* pd3dDebugController = NULL;

	HR(D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&pd3dDebugController));

	if (pd3dDebugController)
	{
		pd3dDebugController->EnableDebugLayer();
		pd3dDebugController->Release();
	}
	nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

#endif


	HR(::CreateDXGIFactory2(nDXGIFactoryFlags, __uuidof(IDXGIFactory4), (void**)&Factory));

	IDXGIAdapter1* pd3dAdapter = NULL;

	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != Factory->EnumAdapters1(i, &pd3dAdapter); i++)
	{
		DXGI_ADAPTER_DESC1 dxgiAdapterDesc;
		pd3dAdapter->GetDesc1(&dxgiAdapterDesc);

		if (dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			continue;

		if (SUCCEEDED(D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void**)&Device)))
			break;
	}


	if (!Device)
	{
		Factory->EnumWarpAdapter(_uuidof(IDXGIFactory4), (void**)&pd3dAdapter);
		HR(D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), (void**)&Device));
	}

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMsaaQualityLevels;
	d3dMsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dMsaaQualityLevels.SampleCount = 4;
	d3dMsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	d3dMsaaQualityLevels.NumQualityLevels = 0;

	HR(Device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &d3dMsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS)));

	Msaa4xQualityLevels = d3dMsaaQualityLevels.NumQualityLevels;
	Msaa4xEnable = (Msaa4xQualityLevels > 1) ? true : false;

	HR(Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&Fence));

	for (UINT i = 0; i < SwapChainBuffers; i++)
		FenceValues[i] = 0;

	FenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	if (pd3dAdapter)
		pd3dAdapter->Release();

	::gnCbvSrvDescriptorIncrementSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	RtvDescriptorIncrementSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	DsvDescriptorIncrementSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

void Engine::CreateCommandQueueAndList()
{
	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc;
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	d3dCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3dCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HR(Device->CreateCommandQueue(&d3dCommandQueueDesc, _uuidof(ID3D12CommandQueue), (void**)&CommandQueue));

	HR(Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&CommandAllocator));

	HR(Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&CommandList));
	HR(CommandList->Close());
}

void Engine::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	d3dDescriptorHeapDesc.NumDescriptors = SwapChainBuffers;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	HR(Device->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&RtvDescriptorHeap));

	d3dDescriptorHeapDesc.NumDescriptors = 1;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	HR(Device->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&DsvDescriptorHeap));
}

void Engine::CreateRenderTargetViews()
{
	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = RtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	for (UINT i = 0; i < SwapChainBuffers; i++)
	{
		SwapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)&SwapChainBackBuffers[i]);
		Device->CreateRenderTargetView(SwapChainBackBuffers[i], NULL, d3dRtvCPUDescriptorHandle);
		d3dRtvCPUDescriptorHandle.ptr += RtvDescriptorIncrementSize;
	}
}

void Engine::CreateDepthStencilView()
{
	D3D12_RESOURCE_DESC d3dResourceDesc;
	d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	d3dResourceDesc.Alignment = 0;
	d3dResourceDesc.Width = WndClientWidth;
	d3dResourceDesc.Height = WndClientHeight;
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

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = DsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	Device->CreateCommittedResource(&d3dHeapProperties, D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &d3dClearValue, __uuidof(ID3D12Resource), (void**)&DepthStencilBuffer);
	/*
		m_pd3dDevice->CreateCommittedResource(&d3dHeapProperties, D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_COMMON, &d3dClearValue, __uuidof(ID3D12Resource), (void **)&m_pd3dDepthStencilBuffer);
		m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, NULL, d3dDsvCPUDescriptorHandle);

		D3D12_RESOURCE_BARRIER d3dResourceBarrier;
		::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
		d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		d3dResourceBarrier.Transition.pResource = m_pd3dDepthStencilBuffer;
		d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
		d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
		d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		m_pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);
	*/

	D3D12_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	::ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D12_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dDepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

	Device->CreateDepthStencilView(DepthStencilBuffer, &d3dDepthStencilViewDesc, d3dDsvCPUDescriptorHandle);
}

void Engine::CreateRenderTargetViewsAndDepthStencilView()
{
	CommandList->Reset(CommandAllocator, NULL);

	CreateRenderTargetViews();
	CreateDepthStencilView();

	CommandList->Close();
	ID3D12CommandList* ppd3dCommandLists[] = { CommandList };
	CommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete();
}

void Engine::ChangeSwapChainState()
{
	WaitForGpuComplete();

	BOOL bFullScreenState = FALSE;
	SwapChain->GetFullscreenState(&bFullScreenState, NULL);
	SwapChain->SetFullscreenState(!bFullScreenState, NULL);

	DXGI_MODE_DESC dxgiTargetParameters;
	dxgiTargetParameters.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiTargetParameters.Width = WndClientWidth;
	dxgiTargetParameters.Height = WndClientHeight;
	dxgiTargetParameters.RefreshRate.Numerator = 60;
	dxgiTargetParameters.RefreshRate.Denominator = 1;
	dxgiTargetParameters.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgiTargetParameters.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChain->ResizeTarget(&dxgiTargetParameters);

	for (int i = 0; i < SwapChainBuffers; i++) if (SwapChainBackBuffers[i]) SwapChainBackBuffers[i]->Release();
#ifdef _WITH_ONLY_RESIZE_BACKBUFFERS
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	m_pdxgiSwapChain->GetDesc(&dxgiSwapChainDesc);
	m_pdxgiSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
#else
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	SwapChain->GetDesc(&dxgiSwapChainDesc);
	SwapChain->ResizeBuffers(SwapChainBuffers, WndClientWidth, WndClientHeight, dxgiSwapChainDesc.BufferDesc.Format, dxgiSwapChainDesc.Flags);
#endif
	SwapChainBufferIndex = SwapChain->GetCurrentBackBufferIndex();

	CreateRenderTargetViews();
}

void Engine::ProcessInput()
{

}

void Engine::OnDestroy()
{
	ReleaseObjects();

	::CloseHandle(FenceEvent);

	if (DepthStencilBuffer)
		DepthStencilBuffer->Release();

	if (DsvDescriptorHeap)
		DsvDescriptorHeap->Release();

	for (int i = 0; i < SwapChainBuffers; i++)
		if (SwapChainBackBuffers[i])
			SwapChainBackBuffers[i]->Release();

	if (RtvDescriptorHeap)
		RtvDescriptorHeap->Release();

	if (CommandAllocator)
		CommandAllocator->Release();

	if (CommandQueue)
		CommandQueue->Release();

	if (CommandList)
		CommandList->Release();

	if (Fence)
		Fence->Release();

	SwapChain->SetFullscreenState(FALSE, NULL);

	if (SwapChain)
		SwapChain->Release();

	if (Device)
		Device->Release();

	if (Factory)
		Factory->Release();

#if defined(_DEBUG)
	if (DebugController)
		DebugController->Release();
#endif
}

void Engine::BuildObjects()
{
	CommandList->Reset(CommandAllocator, NULL);

	this->mScene = new Scene();
	this->mScene->BulidObjects(this->Device, this->CommandList);
	this->mTimer = new Timer();

	//CreateShaderVariables();

	CommandList->Close();
	ID3D12CommandList* ppd3dCommandLists[] = { CommandList };
	CommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete();

	if (this->mScene)
		this->mScene->ReleaseUploadBuffers();
}

void Engine::ReleaseObjects()
{
	//ReleaseShaderVariables();
	if (this->mScene)
		this->mScene->ReleaseObjects();

	if (this->mTimer)
		delete this->mTimer;
}


void Engine::AnimateObjects()
{
	//TODO: Timer만들기
	if (this->mScene)
		this->mScene->AnimateObjects(0.16f);
}

void Engine::CreateShaderVariables()
{
}

void Engine::ReleaseShaderVariables()
{
}

void Engine::OnProcessingMouseMessage(HWND hWnd, UINT MessageID, WPARAM wParam, LPARAM lParam)
{
	switch (MessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		::ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		if (this->mScene)
			this->mScene->OnProcessingMouseMessage(hWnd, MessageID, wParam, lParam);
		break;
	}
}

void Engine::OnProcessingKeyboardMessage(HWND hWnd, UINT MessageID, WPARAM wParam, LPARAM lParam)
{
	switch (MessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		case VK_RETURN:
			break;
		case VK_F1:
			break;
		case VK_F2:
			break;
		case VK_F3:
			break;
		case VK_F9:
			ChangeSwapChainState();
			break;
		case VK_F10:
			break;
		default:
			break;
		}
		break;
	default:
		if (this->mScene)
			this->mScene->OnProcessingKeyboardMessage(hWnd, MessageID, wParam, lParam);
		break;
	}
}

LRESULT CALLBACK Engine::OnProecssingWindowMessage(HWND hWnd, UINT MessageID, WPARAM wParam, LPARAM lParam)
{
	switch (MessageID)
	{
	case WM_ACTIVATE:
	{
		break;
	}
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		if (this->mScene)
			this->mScene->OnProcessingMouseMessage(hWnd, MessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		if (this->mScene)
			this->mScene->OnProcessingKeyboardMessage(hWnd, MessageID, wParam, lParam);
		break;
	}
	return(0);
}

void Engine::UpdateShaderVariables()
{
	/*float fCurrentTime = m_GameTimer.GetTotalTime();
	float fElapsedTime = m_GameTimer.GetTimeElapsed();

	m_pd3dCommandList->SetGraphicsRoot32BitConstants(0, 1, &fCurrentTime, 0);
	m_pd3dCommandList->SetGraphicsRoot32BitConstants(0, 1, &fElapsedTime, 1);

	POINT ptCursorPos;
	::GetCursorPos(&ptCursorPos);
	::ScreenToClient(m_hWnd, &ptCursorPos);
	float fxCursorPos = (ptCursorPos.x < 0) ? 0.0f : float(ptCursorPos.x);
	float fyCursorPos = (ptCursorPos.y < 0) ? 0.0f : float(ptCursorPos.y);

	m_pd3dCommandList->SetGraphicsRoot32BitConstants(0, 1, &fxCursorPos, 2);
	m_pd3dCommandList->SetGraphicsRoot32BitConstants(0, 1, &fyCursorPos, 3);*/
}

void Engine::WaitForGpuComplete()
{
	UINT64 nFenceValue = ++FenceValues[SwapChainBufferIndex];
	HR(CommandQueue->Signal(Fence, nFenceValue));
	if (Fence->GetCompletedValue() < nFenceValue)
	{
		HR(Fence->SetEventOnCompletion(nFenceValue, FenceEvent));
		::WaitForSingleObject(FenceEvent, INFINITE);
	}
}

void Engine::MoveToNextFrame()
{
	SwapChainBufferIndex = SwapChain->GetCurrentBackBufferIndex();
	//m_nSwapChainBufferIndex = (m_nSwapChainBufferIndex + 1) % m_nSwapChainBuffers;

	UINT64 nFenceValue = ++FenceValues[SwapChainBufferIndex];
	HR(CommandQueue->Signal(Fence, nFenceValue));

	if (Fence->GetCompletedValue() < nFenceValue)
	{
		HR(Fence->SetEventOnCompletion(nFenceValue, FenceEvent));
		::WaitForSingleObject(FenceEvent, INFINITE);
	}
}

void Engine::FrameAdvance()
{
	mTimer->Tick();
	// 5초마다 음악 재생/정지 코드
	if ((int)(mTimer->GetnTimeElapsed()) == 5) {
		if (isPaused) {
			mScene->soundmanager->Resume();
			isPaused = false;
		}
		else {
			mScene->soundmanager->Pause();
			isPaused = true;
		}
		mTimer->Reset();
		mTimer->Start();
	}

	ProcessInput();

	AnimateObjects();

	HR(CommandAllocator->Reset());
	HR(CommandList->Reset(CommandAllocator, NULL));

	D3D12_RESOURCE_BARRIER d3dResourceBarrier;
	::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
	d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier.Transition.pResource = SwapChainBackBuffers[SwapChainBufferIndex];
	d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	CommandList->ResourceBarrier(1, &d3dResourceBarrier);

	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = RtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	d3dRtvCPUDescriptorHandle.ptr += (SwapChainBufferIndex * RtvDescriptorIncrementSize);

	float pfClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	CommandList->ClearRenderTargetView(d3dRtvCPUDescriptorHandle, pfClearColor/*Colors::Azure*/, 0, NULL);

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = DsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	CommandList->ClearDepthStencilView(d3dDsvCPUDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

	CommandList->OMSetRenderTargets(1, &d3dRtvCPUDescriptorHandle, TRUE, &d3dDsvCPUDescriptorHandle);

	//TODO : 카메라
	this->mScene->PreRender(this->CommandList, nullptr);
	this->UpdateShaderVariables();

	if (this->mScene)
		this->mScene->Render(this->CommandList/* TODO : 카메라 */);

#ifdef _WITH_PLAYER_TOP
	m_pd3dCommandList->ClearDepthStencilView(d3dDsvCPUDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
#endif


	d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	CommandList->ResourceBarrier(1, &d3dResourceBarrier);

	HR(CommandList->Close());

	ID3D12CommandList* ppd3dCommandLists[] = { CommandList };
	CommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete();

#ifdef _WITH_PRESENT_PARAMETERS
	DXGI_PRESENT_PARAMETERS dxgiPresentParameters;
	dxgiPresentParameters.DirtyRectsCount = 0;
	dxgiPresentParameters.pDirtyRects = NULL;
	dxgiPresentParameters.pScrollRect = NULL;
	dxgiPresentParameters.pScrollOffset = NULL;
	m_pdxgiSwapChain->Present1(1, 0, &dxgiPresentParameters);
#else
#ifdef _WITH_SYNCH_SWAPCHAIN
	m_pdxgiSwapChain->Present(1, 0);
#else
	SwapChain->Present(0, 0);
#endif
#endif

	//	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();
	MoveToNextFrame();

	//m_GameTimer.GetFrameRate(m_pszFrameRate + 12, 37);
	::SetWindowText(Wnd, mFrameRate);
}
