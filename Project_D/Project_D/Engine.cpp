#include "framework.h"
#include "Engine.h"

bool Engine::OnInit(HINSTANCE hInstance, HWND hWnd)
{
	this->hInstance = hInstance;
	this->hWnd = hWnd;

	CreateDirect3DDevice();
	CreateCommandQueueAndList();
	CreateRtvAndDsvDescriptorHeaps();
	CreateSwapChain();
	CreateDepthStencilView();

	BuildObjects();


	return true;
}

void Engine::OnDestroy()
{
	ReleaseObjects();
	::CloseHandle(fenceEvent);

#if defined(_DEBUG)
	SAFE_RELEASE(debugController);
#endif

	for (auto& o : renderTargetBuffers)SAFE_RELEASE(o);
	SAFE_RELEASE(depthStencilBuffer);
	SAFE_RELEASE(depthStencilDescHeap);
	SAFE_RELEASE(renderTargetDescHeap);
	SAFE_RELEASE(commandAllocator);
	SAFE_RELEASE(commandQueue);
	SAFE_RELEASE(commandList);

	SAFE_RELEASE(fence);

	swapChain->SetFullscreenState(false, nullptr);
	//SAFE_RELEASE(swapChain);
	SAFE_RELEASE(factory);
	SAFE_RELEASE(device);
}

void Engine::CreateDirect3DDevice()
{
	// *************************************
	// 디바이스 COM객체를 생성하는 함수입니다.
	// *************************************
#if defined(_DEBUG)

	D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&debugController);
	debugController->EnableDebugLayer();

#endif

	//DXGI 팩토리를 생성한다.
	::CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&factory);

	IDXGIAdapter1* adapter = nullptr;

	//모든 하드웨어 어댑터 대하여 특성 레벨 12.0을 지원하는 하드웨어 디바이스를 생성한다.
	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &adapter); i++)
	{
		DXGI_ADAPTER_DESC1 adapterDesc;
		adapter->GetDesc1(&adapterDesc);

		if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			continue;

		if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void**)&device)))
			break;
	}

	//특성 레벨 12.0을 지원하는 하드웨어 디바이스를 생성할 수 없으면 WARP 디바이스를 생성한다.
	if (!adapter)
	{
		HR(factory->EnumWarpAdapter(_uuidof(IDXGIFactory4), (void**)&adapter));
		HR(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), (void**)&device));
	}

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS MsaaQualityLevels;

	MsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	MsaaQualityLevels.SampleCount = 4; //Msaa4x 다중 샘플링
	MsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	MsaaQualityLevels.NumQualityLevels = 0;
	HR(device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &MsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS)));

	//디바이스가 지원하는 다중 샘플의 품질 수준을 확인한다.
	Msaa4xQualityLevels = MsaaQualityLevels.NumQualityLevels;

	//다중 샘플의 품질 수준이 1보다 크면 다중 샘플링을 활성화한다.
	Msaa4xEnable = (Msaa4xQualityLevels > 1) ? true : false;

	//펜스를 생성하고 펜스 값을 0으로 설정한다.
	HR(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&fence));

	/*
	펜스와 동기화를 위한 이벤트 객체를 생성한다
	(이벤트 객체의 초기값을 FALSE이다).
	이벤트가 실행되면(Signal)이벤트의 값을 자동적으로 FALSE가 되도록 생성한다.
	*/
	fenceEvent = ::CreateEvent(nullptr, false, false, nullptr);

	SAFE_RELEASE(adapter);
}

void Engine::CreateCommandQueueAndList()
{
	// *****************************************************
	// 커맨드큐와 리스트를 만드는 함수입니다.
	// DirectX12부터 생긴 커맨드 리스트는
	// 멀티스레드 렌더링을 할 수 있게 해주는 객체 입니다.
	// 각 커맨드 리스트를 큐에 담아서 GPU에게 할일을 넘겨줍니다.
	// *****************************************************

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;

	::ZeroMemory(&commandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	//직접(Direct) 명령 큐를 생성한다.
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HR(device->CreateCommandQueue(&commandQueueDesc, _uuidof(ID3D12CommandQueue), (void**)&commandQueue));

	//직접(Direct) 명령 할당자를 생성한다.
	HR(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&commandAllocator));

	//직접(Direct) 명령 리스트를 생성한다.
	HR(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, __uuidof(ID3D12GraphicsCommandList), (void**)&commandList));

	//명령 리스트는 생성되면 열린(Open) 상태이므로 닫힌(Closed) 상태로 만든다.
	HR(commandList->Close());
}

void Engine::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	::ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = swapChainBuffers;
	swapChainDesc.BufferDesc.Width = CLIENT_BUFFER_WIDTH;
	swapChainDesc.BufferDesc.Height = CLIENT_BUFFER_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = (Msaa4xEnable) ? 4 : 1; swapChainDesc.SampleDesc.Quality = (Msaa4xEnable) ? (Msaa4xQualityLevels - 1) : 0;
	swapChainDesc.Windowed = true;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HR(factory->CreateSwapChain(commandQueue, &swapChainDesc, (IDXGISwapChain**)&swapChain));
	swapChainBufferIndex = swapChain->GetCurrentBackBufferIndex();
	HR(factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

#ifndef _WITH_SWAPCHAIN_FULLSCREEN_STATE
	CreateRenderTargetView();
#endif

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullScreenDesc{};

	swapChainFullScreenDesc.RefreshRate.Numerator = 60;
	swapChainFullScreenDesc.RefreshRate.Denominator = 1;
	swapChainFullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainFullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainFullScreenDesc.Windowed = true;

	//“Alt+Enter” 키의 동작을 비활성화한다.
	HR(factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

	// 스왑체인의 현재 후면버퍼 인덱스를 저장
	swapChainBufferIndex = swapChain->GetCurrentBackBufferIndex();
}

void Engine::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc;
	::ZeroMemory(&descriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));

	descriptorHeapDesc.NumDescriptors = swapChainBuffers;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	descriptorHeapDesc.NodeMask = 0;

	//렌더 타겟 서술자 힙(서술자의 개수는 스왑체인 버퍼의 개수)을 생성한다
	HR(device->CreateDescriptorHeap(&descriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&renderTargetDescHeap));

	//렌더 타겟 서술자 힙의 원소의 크기를 저장한다.
	renderTargetIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//깊이-스텐실 서술자 힙(서술자의 개수는 1)을 생성한다.
	descriptorHeapDesc.NumDescriptors = 1;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	HR(device->CreateDescriptorHeap(&descriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&depthStencilDescHeap));

	//깊이-스텐실 서술자 힙의 원소의 크기를 저장한다.
	depthStencilIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

void Engine::CreateRenderTargetView()
{
	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = renderTargetDescHeap->GetCPUDescriptorHandleForHeapStart();

	for (UINT i = 0; i < swapChainBuffers; i++)
	{
		HRESULT hResult = swapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)&renderTargetBuffers[i]);

		device->CreateRenderTargetView(renderTargetBuffers[i], nullptr, d3dRtvCPUDescriptorHandle);
		d3dRtvCPUDescriptorHandle.ptr += renderTargetIncrementSize;
	}
}

void Engine::CreateDepthStencilView()
{
	D3D12_RESOURCE_DESC resourceDesc;

	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = CLIENT_BUFFER_WIDTH;
	resourceDesc.Height = CLIENT_BUFFER_HEIGHT;
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

	clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	//깊이-스텐실 버퍼를 생성한다.
	HR(device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue, __uuidof(ID3D12Resource), (void**)&depthStencilBuffer));

	//깊이-스텐실 버퍼 뷰를 생성한다.
	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = depthStencilDescHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateDepthStencilView(depthStencilBuffer, NULL, d3dDsvCPUDescriptorHandle);
}

void Engine::BuildObjects()
{
	commandList->Reset(commandAllocator, nullptr);

	commandList->Close();
	ID3D12CommandList* ppd3dCommandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete();
}

void Engine::ReleaseObjects()
{
}

void Engine::ProcessInput()
{
}

void Engine::AnimateObjects()
{
}

void Engine::FrameAdvance()
{
	HR(device->GetDeviceRemovedReason());

	ProcessInput();
	AnimateObjects();

	//명령 할당자와 명령 리스트를 리셋한다.
	HR(commandAllocator->Reset());
	HR(commandList->Reset(commandAllocator, NULL));

	D3D12_RESOURCE_BARRIER resourceBarrier;
	::ZeroMemory(&resourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));

	resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resourceBarrier.Transition.pResource = renderTargetBuffers[swapChainBufferIndex];
	resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	/*현재 렌더 타겟에 대한 프리젠트가 끝나기를 기다린다. 프리젠트가 끝나면 렌더 타겟 버퍼의 상태는 프리젠트 상태
	(D3D12_RESOURCE_STATE_PRESENT)에서 렌더 타겟 상태(D3D12_RESOURCE_STATE_RENDER_TARGET)로 바뀔 것이다.*/
	commandList->ResourceBarrier(1, &resourceBarrier);

	//현재의 렌더 타겟에 해당하는 서술자의 CPU 주소(핸들)를 계산한다.
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewCPUDescriptorHandle = renderTargetDescHeap->GetCPUDescriptorHandleForHeapStart();
	renderTargetViewCPUDescriptorHandle.ptr += (swapChainBufferIndex * renderTargetIncrementSize);

	//깊이-스텐실 서술자의 CPU 주소를 계산한다.
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilViewCPUDescriptorHandle = depthStencilDescHeap->GetCPUDescriptorHandleForHeapStart();

	//렌더 타겟 뷰(서술자)와 깊이-스텐실 뷰(서술자)를 출력-병합 단계(OM)에 연결한다.
	commandList->OMSetRenderTargets(1, &renderTargetViewCPUDescriptorHandle, true, &depthStencilViewCPUDescriptorHandle);

	//원하는 색상으로 렌더 타겟(뷰)을 지운다.
	float pfClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	commandList->ClearRenderTargetView(renderTargetViewCPUDescriptorHandle, pfClearColor/*Colors::Azure*/, 0, nullptr);

	//원하는 값으로 깊이-스텐실(뷰)을 지운다.
	commandList->ClearDepthStencilView(depthStencilViewCPUDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// 렌더링 코드는 이쪽에 추가한다.


	//현재 렌더 타겟에 대한 렌더링이 끝나기를 기다린다. GPU가 렌더 타겟(버퍼)을 더 이상 사용하지 않으면 렌더 타겟
	//의 상태는 프리젠트 상태(D3D12_RESOURCE_STATE_PRESENT)로 바뀔 것이다.
	resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	commandList->ResourceBarrier(1, &resourceBarrier);

	//명령 리스트를 닫힌 상태로 만든다.
	HR(commandList->Close());

	//명령 리스트를 명령 큐에 추가하여 실행한다.
	ID3D12CommandList* commandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(1, commandLists);

	//GPU가 모든 명령 리스트를 실행할 때 까지 기다린다.
	WaitForGpuComplete();


	/*스왑체인을 프리젠트한다. 프리젠트를 하면 현재 렌더 타겟(후면버퍼)의 내용이 전면버퍼로 옮겨지고 렌더 타겟 인덱스가 바뀔 것이다.*/
	HR(swapChain->Present(0, 0));

	MoveToNextFrame();
}

void Engine::WaitForGpuComplete()
{
	//GPU가 펜스의 값을 설정하는 명령을 명령 큐에 추가한다.
	UINT64 nFenceValue = ++fenceValues[this->swapChainBufferIndex];
	HR(commandQueue->Signal(fence, nFenceValue));

	if (fence->GetCompletedValue() < nFenceValue)
	{
		//펜스의 현재 값이 설정한 값보다 작으면 펜스의 현재 값이 설정한 값이 될 때까지 기다린다.
		HR(fence->SetEventOnCompletion(nFenceValue, fenceEvent));
		::WaitForSingleObject(fenceEvent, INFINITE);
	}
}

void Engine::MoveToNextFrame()
{
	swapChainBufferIndex = swapChain->GetCurrentBackBufferIndex();

	UINT64 fenceValue = ++this->fenceValues[swapChainBufferIndex];
	HR(commandQueue->Signal(fence, fenceValue));

	if (this->fence->GetCompletedValue() < fenceValue)
	{
		HR(fence->SetEventOnCompletion(fenceValue, fenceEvent));
		::WaitForSingleObject(fenceEvent, INFINITE);
	}
}

void Engine::ChangeSwapChainState()
{
	WaitForGpuComplete();

	BOOL bFullScreenState = false;
	swapChain->GetFullscreenState(&bFullScreenState, nullptr);
	swapChain->SetFullscreenState(bFullScreenState, nullptr);

	DXGI_MODE_DESC targetParameters;

	targetParameters.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	targetParameters.Width = CLIENT_BUFFER_WIDTH;
	targetParameters.Height = CLIENT_BUFFER_HEIGHT;
	targetParameters.RefreshRate.Numerator = 60;
	targetParameters.RefreshRate.Denominator = 1;
	targetParameters.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	targetParameters.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	swapChain->ResizeTarget(&targetParameters);

	for (auto& buffer : renderTargetBuffers) SAFE_RELEASE(buffer);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChain->GetDesc(&swapChainDesc);
	swapChain->ResizeBuffers(swapChainBuffers, CLIENT_BUFFER_WIDTH, CLIENT_BUFFER_HEIGHT, swapChainDesc.BufferDesc.Format, swapChainDesc.Flags);

	swapChainBufferIndex = swapChain->GetCurrentBackBufferIndex();

	CreateRenderTargetView();
}

void Engine::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID) {
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}

void Engine::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		case VK_RETURN:
			break;
			//“F9” 키가 눌려지면 윈도우 모드와 전체화면 모드의 전환을 처리한다.
		case VK_F9:
			ChangeSwapChainState();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

LRESULT Engine::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return 0;
}
