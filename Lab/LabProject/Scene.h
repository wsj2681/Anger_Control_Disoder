//-----------------------------------------------------------------------------
// File: Scene.h
//-----------------------------------------------------------------------------
#pragma once

#include "Shader.h"
#include "Player.h"
#include "SoundManager.h"
#include "Particle.h"
#include "EffectBillboard.h"

constexpr UINT MAX_LIGHTS = 50;
constexpr UINT POINT_LIGHT = 1;
constexpr UINT SPOT_LIGHT = 2;
constexpr UINT DIRECTIONAL_LIGHT = 3;

class SkyBox;
class UserInterfaceShader;
class BillboardAnimationShader;
struct LIGHT
{
	XMFLOAT4 m_xmf4Ambient{};
	XMFLOAT4 m_xmf4Diffuse{};
	XMFLOAT4 m_xmf4Specular{};
	XMFLOAT3 m_xmf3Position{};
	float m_fFalloff{};
	XMFLOAT3 m_xmf3Direction{};
	float m_fTheta{}; //cos(m_fTheta)
	XMFLOAT3 m_xmf3Attenuation{};
	float m_fPhi{}; //cos(m_fPhi)
	bool m_bEnable{};
	int m_nType{};
	float m_fRange{};
	float padding{};
};										
										
struct LIGHTS							
{										
	LIGHT m_pLights[MAX_LIGHTS]{};
	XMFLOAT4 m_xmf4GlobalAmbient{};
	int	m_nLights{};
};

class Scene
{
public:
    Scene();
    ~Scene();

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	void BuildDefaultLightsAndMaterials();
	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void Release();

	ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
	ID3D12RootSignature *GetGraphicsRootSignature() { return(m_pd3dGraphicsRootSignature); }

	void Scenario();
	void Hit();

	bool ProcessInput(UCHAR *pKeysBuffer);
    void AnimateObjects(float fTimeElapsed);
    void Render(ID3D12GraphicsCommandList *pd3dCommandList, Camera *pCamera=nullptr);
	void CollideCageSide();
	void CollidePVE();
	void ReleaseUploadBuffers();

	Player								*m_pPlayer = nullptr;

protected:
	ID3D12RootSignature					*m_pd3dGraphicsRootSignature = nullptr;

	static ID3D12DescriptorHeap			*m_pd3dCbvSrvDescriptorHeap;

	static D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dCbvCPUDescriptorStartHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_d3dCbvGPUDescriptorStartHandle;
	static D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dSrvCPUDescriptorStartHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_d3dSrvGPUDescriptorStartHandle;

	static D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dCbvCPUDescriptorNextHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_d3dCbvGPUDescriptorNextHandle;
	static D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dSrvCPUDescriptorNextHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_d3dSrvGPUDescriptorNextHandle;

public:
	static void CreateCbvSrvDescriptorHeaps(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews);

	static D3D12_GPU_DESCRIPTOR_HANDLE CreateConstantBufferViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, ID3D12Resource *pd3dConstantBuffers, UINT nStride);
	static D3D12_GPU_DESCRIPTOR_HANDLE CreateShaderResourceViews(ID3D12Device *pd3dDevice, Texture *pTexture, UINT nRootParameter, bool bAutoIncrement);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUCbvDescriptorStartHandle() { return(m_d3dCbvCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUCbvDescriptorStartHandle() { return(m_d3dCbvGPUDescriptorStartHandle); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSrvDescriptorStartHandle() { return(m_d3dSrvCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSrvDescriptorStartHandle() { return(m_d3dSrvGPUDescriptorStartHandle); }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUCbvDescriptorNextHandle() { return(m_d3dCbvCPUDescriptorNextHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUCbvDescriptorNextHandle() { return(m_d3dCbvGPUDescriptorNextHandle); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSrvDescriptorNextHandle() { return(m_d3dSrvCPUDescriptorNextHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSrvDescriptorNextHandle() { return(m_d3dSrvGPUDescriptorNextHandle); }

	float								m_fElapsedTime = 0.f;

	int									m_nGameObjects = 0;
	Object							**m_ppGameObjects = nullptr;

	vector<Object*> hierarchicalGameObjects;

	int									m_nShaders = 0;
	Shader								**m_ppShaders = nullptr;

	SkyBox								*m_pSkyBox = nullptr;

	LIGHT								*m_pLights = nullptr;
	int									m_nLights = 0;

	XMFLOAT4							m_xmf4GlobalAmbient;

	ID3D12Resource						*m_pd3dcbLights = nullptr;
	LIGHTS								*m_pcbMappedLights = nullptr;

	int lightsCount = 0;
	vector<Object*> lights;

	SoundManager* soundManager{ nullptr };
	SoundManager* hitSound{ nullptr };
	SoundManager* attackSound{ nullptr };

	bool bScenario{ false };

	Object* cageSide = nullptr;

	// 링 바운딩 박스
	BoundingSphere cageCollision;

	Particle* particle;

	map<string, UserInterfaceShader*> ui;
	EffectBillboard* bill = nullptr;
};