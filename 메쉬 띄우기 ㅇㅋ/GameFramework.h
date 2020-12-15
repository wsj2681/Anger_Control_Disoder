#pragma once
#include "framework.h"
#include "Timer.h"
#include "Scene.h"
#include "Camera.h"
#include "Player.h"

class CGameFramework {
    //������ �ڵ�
    HINSTANCE   m_hInstance;
    HWND        m_hWnd;

    //������ â ũ��
    int m_nWndClientWidth{ FRAME_BUFFER_WIDTH };
    int m_nWndClientHeight{ FRAME_BUFFER_HEIGHT };

    IDXGIFactory4* m_pdxgiFactory{ nullptr };  //DXGI ���丮 �������̽� ������ 
    IDXGISwapChain3* m_pdxgiSwapChain{ nullptr };  //����ü�� ������
    ID3D12Device* m_pd3dDevice{ nullptr };  //����̽� ������

    bool m_bMsaa4xEnable{ false };  //���߻��ø� Ȱ��ȭ
    UINT m_nMsaa4xQualityLevels{ 0 };      //���߻��ø� ����

    static const UINT m_nSwapChainBuffers{ 2 };  //����ü�� �ĸ���� ����
    UINT m_nSwapChainBufferIndex{ 0 };  //���� ����ü�� �ĸ� ���� �ε���

    ID3D12Resource* m_ppd3dRenderTargetBuffers[m_nSwapChainBuffers];//���� Ÿ�� ����
    ID3D12DescriptorHeap* m_pd3dRtvDescriptorHeap{ nullptr };      //������ �� �������̽� ������
    UINT                  m_nRtvDescriptorIncrementSize{ 0 };            //����Ÿ�� ������ ������ ũ��

    ID3D12Resource* m_pd3dDepthStencilBuffer{ nullptr };//���� - ���ٽ� ����
    ID3D12DescriptorHeap* m_pd3dDsvDescriptorHeap{ nullptr };//������ �� �������̽� ������
    UINT                  m_nDsvDescriptorIncrementSize{ 0 };      //���� - ���ٽ� ������ ������ ũ��

    ID3D12CommandQueue* m_pd3dCommandQueue{ nullptr };//��� ť
    ID3D12CommandAllocator* m_pd3dCommandAllocator{ nullptr };//��� �Ҵ���
    ID3D12GraphicsCommandList* m_pd3dCommandList{ nullptr };//��� ����Ʈ �������̽� ������

    ID3D12PipelineState* m_pd3dPipelineState{ nullptr };//�׷��Ƚ� ���������λ��� ��ü �������̽� ������

    ID3D12Fence* m_pd3dFence{ nullptr };//�潺 �������̽� ������
    UINT64       m_nFenceValues[m_nSwapChainBuffers]{ 0 };      //�潺 ��

    HANDLE m_hFenceEvent;// �̺�Ʈ �ڵ�

#if defined(_DEBUG)
    ID3D12Debug* m_pd3dDebugController{ nullptr };
#endif
    D3D12_VIEWPORT m_d3dViewport{}; //����Ʈ
    D3D12_RECT m_d3dScissorRect{};//���� �簢��

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

    bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);//�����ӿ�ũ�� �ʱ�ȭ
    void OnDestroy();

    //���� �Լ�
    void CreateSwapChain();// ����ü��
    void CreateDirect3DDevice();//����̽�
    void CreateRtvAndDsvDescriptorHeaps();//������ ��
    void CreateCommandQueueAndList();//���ť/�Ҵ���/����Ʈ
    void CreateRenderTargetView();
    void CreateDepthStencilView();

    //�������� �޽��� ���� ��ü�� �����ϰ� �Ҹ��ϴ� �Լ�
    void BuildObjects();
    void ReleaseObjects();

    //�����ӿ�ũ�� �ٽ�
    void ProcessInput();//������Է�
    void AnimateObjects();//�ִϸ��̼�
    void FrameAdvance();//������

    void WaitForGpuComplete();//CPU�� GPU�� ����ȭ�ϴ� �Լ�

    void ChangeSwapChainState();

    void MoveToNextFrame();

    //������ �޼��� ó�� �Լ�
    void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
    void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

