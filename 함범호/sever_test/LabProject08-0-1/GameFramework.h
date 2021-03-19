#pragma once

#define FRAME_BUFFER_WIDTH		1280
#define FRAME_BUFFER_HEIGHT		1024

#include "Timer.h"
#include "Player.h"
#include "Scene.h"

////SEVER////
#include "Sever.h"

class CGameFramework
{
public:
	CGameFramework()
	{
		m_pdxgiFactory = NULL;
		m_pdxgiSwapChain = NULL;
		m_pd3dDevice = NULL;

		for (int i = 0; i < m_nSwapChainBuffers; i++) m_ppd3dSwapChainBackBuffers[i] = NULL;
		m_nSwapChainBufferIndex = 0;

		m_pd3dCommandAllocator = NULL;
		m_pd3dCommandQueue = NULL;
		m_pd3dCommandList = NULL;

		m_pd3dRtvDescriptorHeap = NULL;
		m_pd3dDsvDescriptorHeap = NULL;

		gnRtvDescriptorIncrementSize = 0;
		gnDsvDescriptorIncrementSize = 0;

		m_hFenceEvent = NULL;
		m_pd3dFence = NULL;
		for (int i = 0; i < m_nSwapChainBuffers; i++) m_nFenceValues[i] = 0;

		m_nWndClientWidth = FRAME_BUFFER_WIDTH;
		m_nWndClientHeight = FRAME_BUFFER_HEIGHT;

		m_pScene = NULL;
		m_pPlayer = NULL;

		_tcscpy_s(m_pszFrameRate, _T("LabProject ("));
	}
	~CGameFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateCommandQueueAndList();

	void CreateRtvAndDsvDescriptorHeaps();

	void CreateRenderTargetViews();
	void CreateDepthStencilView();

	void ChangeSwapChainState();

    void BuildObjects();
    void ReleaseObjects();

    void ProcessInput();
    void AnimateObjects();
    void FrameAdvance();

	void WaitForGpuComplete();
	void MoveToNextFrame();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void CreateFbxSdkManager();

private:
	HINSTANCE					m_hInstance;
	HWND						m_hWnd; 

	int							m_nWndClientWidth;
	int							m_nWndClientHeight;
        
	IDXGIFactory4				*m_pdxgiFactory = NULL;
	IDXGISwapChain3				*m_pdxgiSwapChain = NULL;
	ID3D12Device				*m_pd3dDevice = NULL;

	bool						m_bMsaa4xEnable = false;
	UINT						m_nMsaa4xQualityLevels = 0;

	static const UINT			m_nSwapChainBuffers = 2;
	UINT						m_nSwapChainBufferIndex;

	ID3D12Resource				*m_ppd3dSwapChainBackBuffers[m_nSwapChainBuffers];
	ID3D12DescriptorHeap		*m_pd3dRtvDescriptorHeap = NULL;

	ID3D12Resource				*m_pd3dDepthStencilBuffer = NULL;
	ID3D12DescriptorHeap		*m_pd3dDsvDescriptorHeap = NULL;

	ID3D12CommandAllocator		*m_pd3dCommandAllocator = NULL;
	ID3D12CommandQueue			*m_pd3dCommandQueue = NULL;
	ID3D12GraphicsCommandList	*m_pd3dCommandList = NULL;

	ID3D12Fence					*m_pd3dFence = NULL;
	UINT64						m_nFenceValues[m_nSwapChainBuffers];
	HANDLE						m_hFenceEvent;

#if defined(_DEBUG)
	ID3D12Debug					*m_pd3dDebugController;
#endif

	CGameTimer					m_GameTimer;

	CScene						*m_pScene = NULL;
	CPlayer						*m_pPlayer = NULL;
	CCamera						*m_pCamera = NULL;

	POINT						m_ptOldCursorPos;

	_TCHAR						m_pszFrameRate[70];

	FbxManager* m_pfbxSdkManager = NULL;
	FbxScene* m_pfbxScene = NULL;

	Sever *server= NULL;
};

