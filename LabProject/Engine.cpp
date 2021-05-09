//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Engine.h"
#include "AnimationController.h"

Player* gPlayer = nullptr;

Engine::Engine()
{
	_tcscpy_s(frameRate, _T("AngerControlDisorder("));
}

Engine::~Engine()
{
}

bool Engine::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	this->hInstance = hInstance;
	this->hWnd = hMainWnd;

	CreateDirect3DDevice();
	CreateCommandQueueAndList();
	CreateRtvAndDsvDescriptorHeaps();
	CreateSwapChain();
	CreateDepthStencilView();

	CoInitialize(NULL);

	BuildObjects();

	return(true);
}

void Engine::CreateSwapChain()
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

	HR(factory->CreateSwapChain(commandQueue, &dxgiSwapChainDesc, (IDXGISwapChain **)&swapChain));

	swapChainBufferIndex = swapChain->GetCurrentBackBufferIndex();

	HR(factory->MakeWindowAssociation(this->hWnd, DXGI_MWA_NO_ALT_ENTER));

#ifndef _WITH_SWAPCHAIN_FULLSCREEN_STATE
	CreateRenderTargetViews();
#endif
}

void Engine::CreateDirect3DDevice()
{
	UINT nDXGIFactoryFlags = 0;
#if defined(_DEBUG)
	ID3D12Debug *pd3dDebugController = NULL;
	HR(D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void **)&pd3dDebugController));
	if (pd3dDebugController)
	{
		pd3dDebugController->EnableDebugLayer();
		pd3dDebugController->Release();
	}
	nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	HR(::CreateDXGIFactory2(nDXGIFactoryFlags, __uuidof(IDXGIFactory4), (void **)&factory));

	IDXGIAdapter1 *pd3dAdapter = NULL;

	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &pd3dAdapter); i++)
	{
		DXGI_ADAPTER_DESC1 dxgiAdapterDesc;
		pd3dAdapter->GetDesc1(&dxgiAdapterDesc);
		if (dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;
		if (SUCCEEDED(D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void **)&device))) break;
	}

	if (!pd3dAdapter)
	{
		factory->EnumWarpAdapter(_uuidof(IDXGIFactory4), (void **)&pd3dAdapter);
		HR(D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void **)&device));
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

	HR(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void **)&fence));
	for (UINT i = 0; i < swapChainBuffers; i++) fenceValues[i] = 0;

	fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	if (pd3dAdapter) pd3dAdapter->Release();
}

void Engine::CreateCommandQueueAndList()
{
	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc;
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	d3dCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3dCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HR(device->CreateCommandQueue(&d3dCommandQueueDesc, _uuidof(ID3D12CommandQueue), (void **)&commandQueue));
	
	HR(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void **)&commandAllocator));
	
	HR(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void **)&commandList));
	HR(commandList->Close());
}

void Engine::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	d3dDescriptorHeapDesc.NumDescriptors = swapChainBuffers;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	HR(device->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&rtvDescHeap));
	rtvDescIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	d3dDescriptorHeapDesc.NumDescriptors = 1;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	HR(device->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&dsvDescHeap));
	dsvDescIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

void Engine::CreateRenderTargetViews()
{
	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = rtvDescHeap->GetCPUDescriptorHandleForHeapStart();
	for (UINT i = 0; i < swapChainBuffers; i++)
	{
		swapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void **)&swapChainBackBuffers[i]);
		device->CreateRenderTargetView(swapChainBackBuffers[i], NULL, d3dRtvCPUDescriptorHandle);
		d3dRtvCPUDescriptorHandle.ptr += rtvDescIncrementSize;
	}
}

void Engine::CreateDepthStencilView()
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

	HR(device->CreateCommittedResource(&d3dHeapProperties, D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &d3dClearValue, __uuidof(ID3D12Resource), (void **)&depthStencilBuffer));

	D3D12_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	::ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D12_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dDepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = dsvDescHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateDepthStencilView(depthStencilBuffer, &d3dDepthStencilViewDesc, d3dDsvCPUDescriptorHandle);
}

void Engine::ChangeSwapChainState()
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

void Engine::CreateShaderVariables()
{
	UINT ncbElementBytes = ((sizeof(CB_ENGINE_INFO) + 255) & ~255); //256의 배수
	m_pd3dcbFrameworkInfo = ::CreateBufferResource(device, commandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbFrameworkInfo->Map(0, NULL, (void**)&m_pcbMappedFrameworkInfo);
}

void Engine::UpdateShaderVariables()
{
	m_pcbMappedFrameworkInfo->m_fCurrentTime = m_GameTimer.GetTotalTime();
	m_pcbMappedFrameworkInfo->m_fElapsedTime = m_GameTimer.GetTimeElapsed();

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbFrameworkInfo->GetGPUVirtualAddress();
	commandList->SetGraphicsRootConstantBufferView(16, d3dGpuVirtualAddress);
}

void Engine::ReleaseShaderVariables()
{
	if (m_pd3dcbFrameworkInfo)
	{
		m_pd3dcbFrameworkInfo->Unmap(0, NULL);
		m_pd3dcbFrameworkInfo->Release();
	}
}

void Engine::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pScene) m_pScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
	switch (nMessageID)
	{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			::SetCapture(hWnd);
			::GetCursorPos(&m_ptOldCursorPos);
			break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			::ReleaseCapture();
			break;
		case WM_MOUSEMOVE:
			break;
		default:
			break;
	}
}

void Engine::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pScene) m_pScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
	switch (nMessageID)
	{
		case WM_KEYUP:
			//this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_IDLE);
			this->m_pPlayer->nowState = STATE_IDLE;
			switch (wParam)
			{
				case VK_ESCAPE:
					::PostQuitMessage(0);
					break;
				case VK_RETURN:
					break;
				case VK_F1:
				case VK_F2:
				case VK_F3:
					m_pCamera = m_pPlayer->ChangeCamera((DWORD)(wParam - VK_F1 + 1), m_GameTimer.GetTimeElapsed());
					break;
				case VK_F4:
					if (m_pScene)
						m_pScene->Scenario();
					break;
				case VK_F5:
					cout << "XMFLOAT3(" << m_pPlayer->GetPosition().x << "f, " << m_pPlayer->GetPosition().y << "f, " << m_pPlayer->GetPosition().z << ")" << endl;
					break;
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

LRESULT CALLBACK Engine::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
		case WM_ACTIVATE:
		{
			if (LOWORD(wParam) == WA_INACTIVE)
				m_GameTimer.Stop();
			else
				m_GameTimer.Start();
			break;
		}
		case WM_SIZE:
			break;
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
	return(0);
}

void Engine::OnDestroy()
{
    ReleaseObjects();

	::CloseHandle(fenceEvent);

	if (depthStencilBuffer) depthStencilBuffer->Release();
	if (dsvDescHeap) dsvDescHeap->Release();

	for (int i = 0; i < swapChainBuffers; i++) if (swapChainBackBuffers[i]) swapChainBackBuffers[i]->Release();
	if (rtvDescHeap) rtvDescHeap->Release();

	if (commandAllocator) commandAllocator->Release();
	if (commandQueue) commandQueue->Release();
	if (commandList) commandList->Release();

	if (fence) fence->Release();

	swapChain->SetFullscreenState(FALSE, NULL);
	if (swapChain) swapChain->Release();
    if (device) device->Release();
	if (factory) factory->Release();

#if defined(_DEBUG)
	IDXGIDebug1	*pdxgiDebug = NULL;
	DXGIGetDebugInterface1(0, __uuidof(IDXGIDebug1), (void **)&pdxgiDebug);
	HRESULT hResult = pdxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
	pdxgiDebug->Release();
#endif
}

#define _WITH_TERRAIN_PLAYER

void Engine::BuildObjects()
{

#ifdef _WITH_SERVER_CONNECT
	/////SERVER///
	server = new Server();

	////////////////////////
#endif // _WITH_SERVER_CONNECT


	commandList->Reset(commandAllocator, nullptr);

	m_pScene = new Scene();
	if (m_pScene) m_pScene->BuildObjects(device, commandList);

	BoxingPlayer *pPlayer = new BoxingPlayer(device, commandList, m_pScene->GetGraphicsRootSignature());

	pPlayer->SetPosition(XMFLOAT3(-14.245930f, 10.0f, -769.689f));

	pPlayer->wayPoint.SetWayPoint(XMFLOAT3(-14.245930f, 10.0f, -551.034f), ANIMATION_MOVE_FORWARD);
	pPlayer->wayPoint.SetWayPoint(XMFLOAT3(-14.245930f, 1.66975f, -533.916f), ANIMATION_MOVE_FORWARD);
	pPlayer->wayPoint.SetWayPoint(XMFLOAT3(-14.245930f, -5.69284f, -527.249f), ANIMATION_MOVE_FORWARD);
	pPlayer->wayPoint.SetWayPoint(XMFLOAT3(-14.245930f, -5.69284f, -107.806f), ANIMATION_MOVE_FORWARD);

	pPlayer->wayPoint.SetWayPoint(XMFLOAT3(-81.0648f, -5.69284f, -29.1807f), ANIMATION_CEREMONY);
	pPlayer->wayPoint.SetWayPoint(XMFLOAT3(-77.2785f, -5.69284f, 41.0221f), ANIMATION_CEREMONY);
	//pPlayer->wayPoint.SetWayPoint(XMFLOAT3(-29.7525f, -5.69284f, 81.7311f), ANIMATION_CEREMONY);
	pPlayer->wayPoint.SetWayPoint(XMFLOAT3(-28.4525f, -5.69284f, 83.0311f), ANIMATION_CEREMONY);
	pPlayer->wayPoint.SetWayPoint(XMFLOAT3(37.5937f, -5.69284f, 80.0565f), ANIMATION_CEREMONY);
	pPlayer->wayPoint.SetWayPoint(XMFLOAT3(79.623f, -5.69284f, 31.1354f), ANIMATION_CEREMONY);
	pPlayer->wayPoint.SetWayPoint(XMFLOAT3(81.8642f, -5.69284f, -45.8827f), ANIMATION_CEREMONY);
	pPlayer->wayPoint.SetWayPoint(XMFLOAT3(-17.0f, -5.69284f, -109.177f), ANIMATION_CEREMONY);
	pPlayer->wayPoint.SetWayPoint(XMFLOAT3(-17.0f, -5.69284f, -94.0986f), ANIMATION_MOVE_FORWARD);

	pPlayer->wayPoint.SetWayPoint(XMFLOAT3(-17.0f, 10.0f, -78.1817f), ANIMATION_MOVE_FORWARD);
	pPlayer->wayPoint.SetWayPoint(XMFLOAT3(-17.0f, 10.0f, 0.0f), ANIMATION_MOVE_FORWARD);
	pPlayer->wayPoint.SetWayPoint(XMFLOAT3(0.0f, 10.0f, 0.0f), ANIMATION_MOVE_FORWARD);

	m_pScene->m_pPlayer = m_pPlayer = gPlayer = pPlayer;
	m_pCamera = m_pPlayer->GetCamera();
	m_pScene->hierarchicalGameObjects[1];

#ifdef _WITH_SERVER_CONNECT
	////server//////////////
	server->cplayer = m_pPlayer;
	server->cscene = m_pScene;
	server->cobject = m_pScene->hierarchicalGameObjects[1];
	/// /////////////////////////////
#endif // _WITH_SERVER_CONNECT

	

	commandList->Close();
	ID3D12CommandList *ppd3dCommandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete();

	if (m_pScene) m_pScene->ReleaseUploadBuffers();
	if (m_pPlayer) m_pPlayer->ReleaseUploadBuffers();

	m_GameTimer.Reset();
}

void Engine::ReleaseObjects()
{
	if (m_pPlayer) m_pPlayer->Release();

	if (m_pScene) m_pScene->ReleaseObjects();
	if (m_pScene) delete m_pScene;
}

void Engine::ProcessInput()
{
	static UCHAR pKeysBuffer[256];
	bool bProcessedByScene = false;
	if (GetKeyboardState(pKeysBuffer) && m_pScene) bProcessedByScene = m_pScene->ProcessInput(pKeysBuffer);
	if (!bProcessedByScene)
	{
		DWORD dwDirection = 0;

		if (m_pPlayer)
		{
			if (m_pPlayer->bScenario == false)
			{
				//// temp 이동키
				//if (pKeysBuffer['S'] & 0xF0)
				//{
				//	this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, pKeysBuffer['S'] & 0xF0 ? ANIMATION_MOVE_BACKWARD : ANIMATION_IDLE);
				//	dwDirection |= DIR_BACKWARD;
				//}
				//if (pKeysBuffer['A'] & 0xF0)
				//{
				//	this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, pKeysBuffer['A'] & 0xF0 ? ANIMATION_MOVE_LEFT : ANIMATION_IDLE);
				//	dwDirection |= DIR_LEFT;
				//}
				//if (pKeysBuffer['D'] & 0xF0)
				//{
				//	this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, pKeysBuffer['D'] & 0xF0 ? ANIMATION_MOVE_RIGHT : ANIMATION_IDLE);
				//	dwDirection |= DIR_RIGHT;
				//}
				//if (pKeysBuffer['W'] & 0xF0)
				//{
				//	this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, pKeysBuffer['W'] & 0xF0 ? ANIMATION_MOVE_FORWARD : ANIMATION_IDLE);
				//	dwDirection |= DIR_FORWARD;
				//}
				//if (pKeysBuffer[VK_SPACE] & 0xF0) dwDirection |= DIR_UP;
				//if (pKeysBuffer[VK_RSHIFT] & 0xF0) dwDirection |= DIR_DOWN;


				 //여기가 게임 키
				if (pKeysBuffer[VK_SPACE] & 0xF0)
				{
					this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, pKeysBuffer[VK_SPACE] & 0xF0 ? ANIMATION_MOVE_BACKWARD : ANIMATION_IDLE);
					dwDirection |= DIR_BACKWARD;
				}
				if (pKeysBuffer[VK_LSHIFT] & 0xF0)
				{
					this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, pKeysBuffer[VK_LSHIFT] & 0xF0 ? ANIMATION_MOVE_LEFT : ANIMATION_IDLE);
					dwDirection |= DIR_LEFT;

				}
				if (pKeysBuffer[VK_RSHIFT] & 0xF0)
				{
					this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, pKeysBuffer[VK_RSHIFT] & 0xF0 ? ANIMATION_MOVE_RIGHT : ANIMATION_IDLE);
					dwDirection |= DIR_RIGHT;
				}
				if (pKeysBuffer[VK_LSHIFT] & pKeysBuffer[VK_RSHIFT] & 0xF0)
				{
					this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, pKeysBuffer[VK_LSHIFT] & pKeysBuffer[VK_RSHIFT] & 0xF0 ? ANIMATION_MOVE_FORWARD : ANIMATION_IDLE);
					dwDirection |= DIR_FORWARD;
				}
				//if (pKeysBuffer[VK_SPACE] & 0xF0) dwDirection |= DIR_UP;
				//if (pKeysBuffer[VK_RSHIFT] & 0xF0) dwDirection |= DIR_DOWN;

				if (pKeysBuffer['A'] & 0xF0)
				{
					this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HOOK_L);
					this->m_pPlayer->nowState = STATE_ATTACK_LEFT_HOOK;
#ifdef _WITH_SERVER_CONNECT
					server->send_attackAnddefend.leftHand = true;
#endif // _WITH_SERVER_CONNECT
				}
				if (pKeysBuffer[VK_OEM_7] & 0xF0)
				{
					this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, pKeysBuffer[VK_OEM_7] & 0xF0 ? ANIMATION_HOOK_R : ANIMATION_IDLE);
					this->m_pPlayer->nowState = STATE_ATTACK_RIGHT_HOOK;
#ifdef _WITH_SERVER_CONNECT
					server->send_attackAnddefend.rightHand = true;
#endif // _WITH_SERVER_CONNECT
				}
				if (pKeysBuffer['S'] & 0xF0)
				{
					this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, pKeysBuffer['S'] & 0xF0 ? ANIMATION_JAB : ANIMATION_IDLE);
					this->m_pPlayer->nowState = STATE_ATTACK_JAB;
#ifdef _WITH_SERVER_CONNECT
					server->send_attackAnddefend.jap = true;
#endif // _WITH_SERVER_CONNECT
				}
				if (pKeysBuffer['C'] & 0xF0)
				{
					this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, pKeysBuffer['C'] & 0xF0 ? ANIMATION_GUARD_LEFT_HEAD : ANIMATION_IDLE);
					this->m_pPlayer->nowState = STATE_GUARD_LEFT_HEAD;
#ifdef _WITH_SERVER_CONNECT
					server->send_attackAnddefend.leftGuard = true;
#endif // _WITH_SERVER_CONNECT
				}
				if (pKeysBuffer[VK_OEM_COMMA] & 0xF0)
				{
					this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, pKeysBuffer[VK_OEM_COMMA] & 0xF0 ? ANIMATION_GUARD_RIGHT_HEAD : ANIMATION_IDLE);
					this->m_pPlayer->nowState = STATE_GUARD_RIGHT_HEAD;
#ifdef _WITH_SERVER_CONNECT
					server->send_attackAnddefend.rightGuard = true;
#endif // _WITH_SERVER_CONNECT
				}
				if (pKeysBuffer['6'] & 0xF0)
				{
					this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, pKeysBuffer['6'] & 0xF0 ? ANIMATION_GUARD_BODY : ANIMATION_IDLE);
					this->m_pPlayer->nowState = STATE_GUARD_BODY;
#ifdef _WITH_SERVER_CONNECT
					server->send_attackAnddefend.middleGuard = true;
#endif // _WITH_SERVER_CONNECT
				}
				if (pKeysBuffer['7'] & 0xF0)
				{
					this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, pKeysBuffer['7'] & 0xF0 ? ANIMATION_HIT_TORSO_LEFT_A : ANIMATION_IDLE);
					this->m_pPlayer->nowState = STATE_HIT_TORSO_LEFT;
#ifdef _WITH_SERVER_CONNECT
					server->send_attackAnddefend.hitTorsoLeft = true;
#endif // _WITH_SERVER_CONNECT
				}
				if (pKeysBuffer['8'] & 0xF0)
				{
					this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, pKeysBuffer['8'] & 0xF0 ? ANIMATION_HIT_TORSO_RIGHT_A : ANIMATION_IDLE);
					this->m_pPlayer->nowState = STATE_HIT_TORSO_RIGHT;
#ifdef _WITH_SERVER_CONNECT
					server->send_attackAnddefend.hitTorsoRight = true;
#endif // _WITH_SERVER_CONNECT
				}
				if (pKeysBuffer['9'] & 0xF0)
				{
					this->m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, pKeysBuffer['9'] & 0xF0 ? ANIMATION_HIT_TORSO_STRIGHT_A : ANIMATION_IDLE);
					this->m_pPlayer->nowState = STATE_HIT_TORSO_STRIGHT;
#ifdef _WITH_SERVER_CONNECT
					server->send_attackAnddefend.hitTorsoStright = true;
#endif // _WITH_SERVER_CONNECT
				}

				float cxDelta = 0.0f, cyDelta = 0.0f;
				POINT ptCursorPos;
				if (GetCapture() == this->hWnd)
				{
					SetCursor(NULL);
					GetCursorPos(&ptCursorPos);
					cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
					cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
					SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
				}

				if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
				{
					if (cxDelta || cyDelta)
					{
						if (pKeysBuffer[VK_RBUTTON] & 0xF0)
							m_pPlayer->Rotate(cyDelta, 0.0f, -cxDelta);
						else
							m_pPlayer->Rotate(cyDelta, cxDelta, 0.0f);
					}
					if (dwDirection) m_pPlayer->Move(dwDirection, 50.25f, true);
				}
			}

			m_pPlayer->Update(m_GameTimer.GetTimeElapsed());
		}

	}
}

void Engine::AnimateObjects()
{
	float fTimeElapsed = m_GameTimer.GetTimeElapsed();

	if (m_pScene) m_pScene->AnimateObjects(fTimeElapsed);

	m_pPlayer->Animate(fTimeElapsed);
}

void Engine::WaitForGpuComplete()
{
	const UINT64 nFenceValue = ++fenceValues[swapChainBufferIndex];
	HR(commandQueue->Signal(fence, nFenceValue));

	if (fence->GetCompletedValue() < nFenceValue)
	{
		HR(fence->SetEventOnCompletion(nFenceValue, fenceEvent));
		::WaitForSingleObject(fenceEvent, INFINITE);
	}
}

void Engine::MoveToNextFrame()
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

//#define _WITH_PLAYER_TOP

void Engine::FrameAdvance()
{    
#ifdef _WITH_SERVER_CONNECT
	/////////////////server////////////////
	//if (i == 0) {
	server->Server_send();
	server->Server_recv();
	//++i;
	//}
	//공격과 방어 초기화
	server->attackAndGuard_idle();
	//server->Server_send();

	///////////////////////////////////////
#endif // _WITH_SERVER_CONNECT

	m_GameTimer.Tick(60.0f);
	
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

	if (m_pScene) m_pScene->Render(commandList, m_pCamera);

#ifdef _WITH_PLAYER_TOP
	commandList->ClearDepthStencilView(d3dDsvCPUDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
#endif
	if (m_pPlayer) m_pPlayer->Render(commandList, m_pCamera);

	d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	commandList->ResourceBarrier(1, &d3dResourceBarrier);

	HR(commandList->Close());
	
	ID3D12CommandList *ppd3dCommandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete();

	swapChain->Present(0, 0);

	MoveToNextFrame();

	m_GameTimer.GetFrameRate(frameRate + 12, 37);
	size_t nLength = _tcslen(frameRate);
	XMFLOAT3 xmf3Position = m_pPlayer->GetPosition();
	_stprintf_s(frameRate + nLength, 70 - nLength, _T("(%4f, %4f, %4f)"), xmf3Position.x, xmf3Position.y, xmf3Position.z);
	::SetWindowText(this->hWnd, frameRate);
}

