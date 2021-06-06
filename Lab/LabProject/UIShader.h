#pragma once
#include "Shader.h"
#include "Texture.h"

class UserInterfaceShader : public Shader
{
public:
	UserInterfaceShader();
	UserInterfaceShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);
	virtual ~UserInterfaceShader();

protected:

	Texture* m_pTexture = nullptr;

	bool active = true;


public:
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();
	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);

	void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera);
	bool isActive() const { return this->active; }
	void SetActive(bool active) { this->active = active; }
};

struct VS_CB_HP_INFO
{
	float hp = 0;
};

class UI_HP_Player : public UserInterfaceShader
{
public:
	UI_HP_Player();
	UI_HP_Player(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);
	virtual ~UI_HP_Player();

private:

	ID3D12Resource* m_pd3dcbHPInfo = NULL;
	VS_CB_HP_INFO* m_pcbMappedHPInfo = NULL;

public:

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();
	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);
};

class UI_HP_OtherPlayer : public UserInterfaceShader
{
public:
	UI_HP_OtherPlayer();
	UI_HP_OtherPlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);
	virtual ~UI_HP_OtherPlayer();

private:

	ID3D12Resource* m_pd3dcbHPInfo = NULL;
	VS_CB_HP_INFO* m_pcbMappedHPInfo = NULL;

public:

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();
	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);
};

class UI_KeyInput_Right_Shift : public UserInterfaceShader
{
public:

	UI_KeyInput_Right_Shift();
	UI_KeyInput_Right_Shift(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);
	virtual ~UI_KeyInput_Right_Shift();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();
	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);
};

class UI_KeyInput_Left_Shift : public UserInterfaceShader
{
public:

	UI_KeyInput_Left_Shift();
	UI_KeyInput_Left_Shift(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);
	virtual ~UI_KeyInput_Left_Shift();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();
	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);
};

class UI_KeyInput_Space : public UserInterfaceShader
{
public:

	UI_KeyInput_Space();
	UI_KeyInput_Space(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);
	virtual ~UI_KeyInput_Space();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();
	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);
};

class UI_PlayerTotalScore : public UserInterfaceShader
{
public:

	UI_PlayerTotalScore();
	UI_PlayerTotalScore(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);
	virtual ~UI_PlayerTotalScore();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();
	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);
};

class UI_OtherPlayerTotalScore : public UserInterfaceShader
{
public:

	UI_OtherPlayerTotalScore();
	UI_OtherPlayerTotalScore(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);
	virtual ~UI_OtherPlayerTotalScore();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();
	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath);
};