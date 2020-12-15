#pragma once
#include "Timer.h"
#include "Shader.h"
#include "Camera.h"

class CScene{
protected:
    ID3D12RootSignature* m_pd3dGraphicsRootSignature{ nullptr };

    //씬은 셰이더들의 집합이다. 셰이더들은 게임 객체들의 집합이다.
    //CShader** m_ppShaders       { nullptr };
    //int m_nShaders              { 0 };


    CGameObject** m_ppObjects   { nullptr };
    int m_nObjects              { 0 };

public:
    CScene();
    ~CScene();
    
    bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
    bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
    
    void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
    void ReleaseObjects();
    
    bool ProcessInput();
    void AnimateObjects(float fTimeElapsed);
    void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
   
    void ReleaseUploadBuffers();

    //그래픽 루트 시그너쳐를 생성한다.
    ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
    ID3D12RootSignature* GetGraphicsRootSignature();
};

