#pragma once
#include "GameObject.h"
#include "Camera.h"

struct CB_GAMEOBJECT_INFO
{
    XMFLOAT4X4 m_xmf4x4World{ Matrix4x4::Identity() };
};

class CShader{
private:
    int m_nReferences{ 0 };

protected:
    //셰이더가 포함하는 게임 객체들의 리스트(배열)이다.
    CGameObject** m_ppObjects{ nullptr };
    int m_nObjects           { 0 };

    //파이프라인 상태 객체들의 리스트(배열)이다.
    ID3D12PipelineState** m_ppd3dPipelineStates{ nullptr };
    int m_nPipelineStates   { 0 };

public:
    CShader();
    virtual ~CShader();
    void AddRef() { m_nReferences++; }
    void Release() { if (--m_nReferences <= 0) delete this; }
    
    virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
    virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
    virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
    virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
    virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
    virtual D3D12_BLEND_DESC CreateBlendState();
    
    D3D12_SHADER_BYTECODE CompileShaderFromFile(LPCWSTR pszFileName, LPCSTR pszShaderName,LPCSTR pszShaderProfile, ID3DBlob** ppd3dShaderBlob);
    
    virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pContext = NULL);
    virtual void ReleaseObjects();
    virtual void AnimateObjects(float fTimeElapsed);
    
    virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dRootSignature);
    virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
   
    virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
    virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);
    
    virtual void ReleaseShaderVariables();
    virtual void ReleaseUploadBuffers();
    
    virtual void OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
    virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
};

class CDiffusedShader : public CShader{
public:
    CDiffusedShader();
    virtual ~CDiffusedShader();
    virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
    virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
    virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
    virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature);
};

class CObjectsShader : public CShader {
protected:
    CGameObject** m_ppObjects{ nullptr };
    int m_nObjects{ 0 };
public:
    CObjectsShader();
    virtual ~CObjectsShader();

    virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
    virtual void AnimateObjects(float fTimeElapsed);
    virtual void ReleaseObjects();

    virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
    virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
    virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
    virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature);

    virtual void ReleaseUploadBuffers();
    virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
};