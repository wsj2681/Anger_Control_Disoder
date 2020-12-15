#pragma once
#include "Timer.h"
#include "Shader.h"
#include "Camera.h"

class CScene{
protected:
    ID3D12RootSignature* m_pd3dGraphicsRootSignature{ nullptr };

    //���� ���̴����� �����̴�. ���̴����� ���� ��ü���� �����̴�.
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

    //�׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�.
    ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
    ID3D12RootSignature* GetGraphicsRootSignature();
};

