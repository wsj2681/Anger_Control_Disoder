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
	// ����̽� COM��ü�� �����ϴ� �Լ��Դϴ�.
	// *************************************
#if defined(_DEBUG)

	D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&debugController);
	debugController->EnableDebugLayer();

#endif

	//DXGI ���丮�� �����Ѵ�.
	::CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&factory);

	IDXGIAdapter1* adapter = nullptr;

	//��� �ϵ���� ����� ���Ͽ� Ư�� ���� 12.0�� �����ϴ� �ϵ���� ����̽��� �����Ѵ�.
	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &adapter); i++)
	{
		DXGI_ADAPTER_DESC1 adapterDesc;
		adapter->GetDesc1(&adapterDesc);

		if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			continue;

		if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void**)&device)))
			break;
	}

	//Ư�� ���� 12.0�� �����ϴ� �ϵ���� ����̽��� ������ �� ������ WARP ����̽��� �����Ѵ�.
	if (!adapter)
	{
		HR(factory->EnumWarpAdapter(_uuidof(IDXGIFactory4), (void**)&adapter));
		HR(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), (void**)&device));
	}

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS MsaaQualityLevels;

	MsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	MsaaQualityLevels.SampleCount = 4; //Msaa4x ���� ���ø�
	MsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	MsaaQualityLevels.NumQualityLevels = 0;
	HR(device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &MsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS)));

	//����̽��� �����ϴ� ���� ������ ǰ�� ������ Ȯ���Ѵ�.
	Msaa4xQualityLevels = MsaaQualityLevels.NumQualityLevels;

	//���� ������ ǰ�� ������ 1���� ũ�� ���� ���ø��� Ȱ��ȭ�Ѵ�.
	Msaa4xEnable = (Msaa4xQualityLevels > 1) ? true : false;

	//�潺�� �����ϰ� �潺 ���� 0���� �����Ѵ�.
	HR(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&fence));

	/*
	�潺�� ����ȭ�� ���� �̺�Ʈ ��ü�� �����Ѵ�
	(�̺�Ʈ ��ü�� �ʱⰪ�� FALSE�̴�).
	�̺�Ʈ�� ����Ǹ�(Signal)�̺�Ʈ�� ���� �ڵ������� FALSE�� �ǵ��� �����Ѵ�.
	*/
	fenceEvent = ::CreateEvent(nullptr, false, false, nullptr);

	SAFE_RELEASE(adapter);
}

void Engine::CreateCommandQueueAndList()
{
	// *****************************************************
	// Ŀ�ǵ�ť�� ����Ʈ�� ����� �Լ��Դϴ�.
	// DirectX12���� ���� Ŀ�ǵ� ����Ʈ��
	// ��Ƽ������ �������� �� �� �ְ� ���ִ� ��ü �Դϴ�.
	// �� Ŀ�ǵ� ����Ʈ�� ť�� ��Ƽ� GPU���� ������ �Ѱ��ݴϴ�.
	// *****************************************************

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;

	::ZeroMemory(&commandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	//����(Direct) ��� ť�� �����Ѵ�.
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HR(device->CreateCommandQueue(&commandQueueDesc, _uuidof(ID3D12CommandQueue), (void**)&commandQueue));

	//����(Direct) ��� �Ҵ��ڸ� �����Ѵ�.
	HR(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&commandAllocator));

	//����(Direct) ��� ����Ʈ�� �����Ѵ�.
	HR(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, __uuidof(ID3D12GraphicsCommandList), (void**)&commandList));

	//��� ����Ʈ�� �����Ǹ� ����(Open) �����̹Ƿ� ����(Closed) ���·� �����.
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

	//��Alt+Enter�� Ű�� ������ ��Ȱ��ȭ�Ѵ�.
	HR(factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

	// ����ü���� ���� �ĸ���� �ε����� ����
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

	//���� Ÿ�� ������ ��(�������� ������ ����ü�� ������ ����)�� �����Ѵ�
	HR(device->CreateDescriptorHeap(&descriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&renderTargetDescHeap));

	//���� Ÿ�� ������ ���� ������ ũ�⸦ �����Ѵ�.
	renderTargetIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//����-���ٽ� ������ ��(�������� ������ 1)�� �����Ѵ�.
	descriptorHeapDesc.NumDescriptors = 1;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	HR(device->CreateDescriptorHeap(&descriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&depthStencilDescHeap));

	//����-���ٽ� ������ ���� ������ ũ�⸦ �����Ѵ�.
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

	//����-���ٽ� ���۸� �����Ѵ�.
	HR(device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue, __uuidof(ID3D12Resource), (void**)&depthStencilBuffer));

	//����-���ٽ� ���� �並 �����Ѵ�.
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

	//��� �Ҵ��ڿ� ��� ����Ʈ�� �����Ѵ�.
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

	/*���� ���� Ÿ�ٿ� ���� ������Ʈ�� �����⸦ ��ٸ���. ������Ʈ�� ������ ���� Ÿ�� ������ ���´� ������Ʈ ����
	(D3D12_RESOURCE_STATE_PRESENT)���� ���� Ÿ�� ����(D3D12_RESOURCE_STATE_RENDER_TARGET)�� �ٲ� ���̴�.*/
	commandList->ResourceBarrier(1, &resourceBarrier);

	//������ ���� Ÿ�ٿ� �ش��ϴ� �������� CPU �ּ�(�ڵ�)�� ����Ѵ�.
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewCPUDescriptorHandle = renderTargetDescHeap->GetCPUDescriptorHandleForHeapStart();
	renderTargetViewCPUDescriptorHandle.ptr += (swapChainBufferIndex * renderTargetIncrementSize);

	//����-���ٽ� �������� CPU �ּҸ� ����Ѵ�.
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilViewCPUDescriptorHandle = depthStencilDescHeap->GetCPUDescriptorHandleForHeapStart();

	//���� Ÿ�� ��(������)�� ����-���ٽ� ��(������)�� ���-���� �ܰ�(OM)�� �����Ѵ�.
	commandList->OMSetRenderTargets(1, &renderTargetViewCPUDescriptorHandle, true, &depthStencilViewCPUDescriptorHandle);

	//���ϴ� �������� ���� Ÿ��(��)�� �����.
	float pfClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	commandList->ClearRenderTargetView(renderTargetViewCPUDescriptorHandle, pfClearColor/*Colors::Azure*/, 0, nullptr);

	//���ϴ� ������ ����-���ٽ�(��)�� �����.
	commandList->ClearDepthStencilView(depthStencilViewCPUDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// ������ �ڵ�� ���ʿ� �߰��Ѵ�.


	//���� ���� Ÿ�ٿ� ���� �������� �����⸦ ��ٸ���. GPU�� ���� Ÿ��(����)�� �� �̻� ������� ������ ���� Ÿ��
	//�� ���´� ������Ʈ ����(D3D12_RESOURCE_STATE_PRESENT)�� �ٲ� ���̴�.
	resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	commandList->ResourceBarrier(1, &resourceBarrier);

	//��� ����Ʈ�� ���� ���·� �����.
	HR(commandList->Close());

	//��� ����Ʈ�� ��� ť�� �߰��Ͽ� �����Ѵ�.
	ID3D12CommandList* commandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(1, commandLists);

	//GPU�� ��� ��� ����Ʈ�� ������ �� ���� ��ٸ���.
	WaitForGpuComplete();


	/*����ü���� ������Ʈ�Ѵ�. ������Ʈ�� �ϸ� ���� ���� Ÿ��(�ĸ����)�� ������ ������۷� �Ű����� ���� Ÿ�� �ε����� �ٲ� ���̴�.*/
	HR(swapChain->Present(0, 0));

	MoveToNextFrame();
}

void Engine::WaitForGpuComplete()
{
	//GPU�� �潺�� ���� �����ϴ� ����� ��� ť�� �߰��Ѵ�.
	UINT64 nFenceValue = ++fenceValues[this->swapChainBufferIndex];
	HR(commandQueue->Signal(fence, nFenceValue));

	if (fence->GetCompletedValue() < nFenceValue)
	{
		//�潺�� ���� ���� ������ ������ ������ �潺�� ���� ���� ������ ���� �� ������ ��ٸ���.
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
			//��F9�� Ű�� �������� ������ ���� ��üȭ�� ����� ��ȯ�� ó���Ѵ�.
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
