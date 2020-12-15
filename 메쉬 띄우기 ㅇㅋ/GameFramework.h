#pragma once
#include "framework.h"
#include "Timer.h"
#include "Scene.h"
#include "Camera.h"
#include "Player.h"

class CGameFramework {
    //윈도우 핸들
    HINSTANCE   m_hInstance;
    HWND        m_hWnd;

    //윈도우 창 크기
    int m_nWndClientWidth{ FRAME_BUFFER_WIDTH };
    int m_nWndClientHeight{ FRAME_BUFFER_HEIGHT };

    IDXGIFactory4* m_pdxgiFactory{ nullptr };  //DXGI 팩토리 인터페이스 포인터 
    IDXGISwapChain3* m_pdxgiSwapChain{ nullptr };  //스왑체인 포인터
    ID3D12Device* m_pd3dDevice{ nullptr };  //디바이스 포인터

    bool m_bMsaa4xEnable{ false };  //다중샘플링 활성화
    UINT m_nMsaa4xQualityLevels{ 0 };      //다중샘플링 레벨

    static const UINT m_nSwapChainBuffers{ 2 };  //스왑체인 후면버퍼 개수
    UINT m_nSwapChainBufferIndex{ 0 };  //현재 스왑체인 후면 버퍼 인덱스

    ID3D12Resource* m_ppd3dRenderTargetBuffers[m_nSwapChainBuffers];//렌더 타겟 버퍼
    ID3D12DescriptorHeap* m_pd3dRtvDescriptorHeap{ nullptr };      //서술자 힙 인터페이스 포인터
    UINT                  m_nRtvDescriptorIncrementSize{ 0 };            //렌더타겟 서술자 원소의 크기

    ID3D12Resource* m_pd3dDepthStencilBuffer{ nullptr };//깊이 - 스텐실 버퍼
    ID3D12DescriptorHeap* m_pd3dDsvDescriptorHeap{ nullptr };//서술자 힙 인터페이스 포인터
    UINT                  m_nDsvDescriptorIncrementSize{ 0 };      //깊이 - 스텐실 서술자 원소의 크기

    ID3D12CommandQueue* m_pd3dCommandQueue{ nullptr };//명령 큐
    ID3D12CommandAllocator* m_pd3dCommandAllocator{ nullptr };//명령 할당자
    ID3D12GraphicsCommandList* m_pd3dCommandList{ nullptr };//명령 리스트 인터페이스 포인터

    ID3D12PipelineState* m_pd3dPipelineState{ nullptr };//그래픽스 파이프라인상태 객체 인터페이스 포인터

    ID3D12Fence* m_pd3dFence{ nullptr };//펜스 인터페이스 포인터
    UINT64       m_nFenceValues[m_nSwapChainBuffers]{ 0 };      //펜스 값

    HANDLE m_hFenceEvent;// 이벤트 핸들

#if defined(_DEBUG)
    ID3D12Debug* m_pd3dDebugController{ nullptr };
#endif
    D3D12_VIEWPORT m_d3dViewport{}; //뷰포트
    D3D12_RECT m_d3dScissorRect{};//시저 사각형

    //Timer
    CGameTimer m_GameTimer{};
    _TCHAR m_pszFrameRate[50];

    //Scene
    CScene* m_pScene{ nullptr };
    CCamera* m_pCamera{ nullptr };
    CPlayer* m_pPlayer{ nullptr };

    POINT m_ptOldCursorPos{};
public:
    CGameFramework();
    ~CGameFramework();

    bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);//프레임워크를 초기화
    void OnDestroy();

    //생성 함수
    void CreateSwapChain();// 스왑체인
    void CreateDirect3DDevice();//디바이스
    void CreateRtvAndDsvDescriptorHeaps();//서술자 힙
    void CreateCommandQueueAndList();//명령큐/할당자/리스트
    void CreateRenderTargetView();
    void CreateDepthStencilView();

    //렌더링할 메쉬와 게임 객체를 생성하고 소멸하는 함수
    void BuildObjects();
    void ReleaseObjects();

    //프레임워크의 핵심
    void ProcessInput();//사용자입력
    void AnimateObjects();//애니메이션
    void FrameAdvance();//렌더링

    void WaitForGpuComplete();//CPU와 GPU를 동기화하는 함수

    void ChangeSwapChainState();

    void MoveToNextFrame();

    //윈도우 메세지 처리 함수
    void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
    void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

