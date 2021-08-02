#include "stdafx.h"
#include "UIShader.h"
#include "Scene.h"

extern Scene* gScene;

UserInterfaceShader::UserInterfaceShader()
{
}

UserInterfaceShader::UserInterfaceShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	BuildObjects(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, filePath);
}

UserInterfaceShader::~UserInterfaceShader()
{
}

D3D12_INPUT_LAYOUT_DESC UserInterfaceShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE UserInterfaceShader::CreateVertexShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSTextureUI", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_SHADER_BYTECODE UserInterfaceShader::CreatePixelShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSTextureUI", "ps_5_1", &m_pd3dPixelShaderBlob));
}

void UserInterfaceShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filePath, 0);
	Scene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 17, false);
}

void UserInterfaceShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	if (active)
	{
		m_pTexture->UpdateShaderVariables(pd3dCommandList);
		Shader::Render(pd3dCommandList, pCamera);

		pd3dCommandList->DrawInstanced(6, 1, 0, 0);
	}
}

UI_HP_Player::UI_HP_Player()
{
}

UI_HP_Player::UI_HP_Player(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	BuildObjects(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, filePath);
}

UI_HP_Player::~UI_HP_Player()
{
}

void UI_HP_Player::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(VS_CB_HP_INFO) + 255) & ~255); //256의 배수
	m_pd3dcbHPInfo = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbHPInfo->Map(0, NULL, (void**)&m_pcbMappedHPInfo);
}

void UI_HP_Player::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	::memcpy(&m_pcbMappedHPInfo->hp, &gScene->m_pPlayer->hp, sizeof(float));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbHPInfo->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(18, d3dGpuVirtualAddress);
}

void UI_HP_Player::ReleaseShaderVariables()
{
	if (m_pd3dcbHPInfo)
	{
		m_pd3dcbHPInfo->Unmap(0, NULL);
		m_pd3dcbHPInfo->Release();
	}
}

D3D12_INPUT_LAYOUT_DESC UI_HP_Player::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE UI_HP_Player::CreateVertexShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSTextureUI_HP", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_SHADER_BYTECODE UI_HP_Player::CreatePixelShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSTextureUI_HP", "ps_5_1", &m_pd3dPixelShaderBlob));
}

void UI_HP_Player::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filePath, 0);
	Scene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 17, false);
}

UI_HP_OtherPlayer::UI_HP_OtherPlayer()
{
}

UI_HP_OtherPlayer::UI_HP_OtherPlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	BuildObjects(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, filePath);
}

UI_HP_OtherPlayer::~UI_HP_OtherPlayer()
{
}

void UI_HP_OtherPlayer::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(VS_CB_HP_INFO) + 255) & ~255); //256의 배수
	m_pd3dcbHPInfo = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbHPInfo->Map(0, NULL, (void**)&m_pcbMappedHPInfo);
}

void UI_HP_OtherPlayer::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	// other player hp info
	::memcpy(&m_pcbMappedHPInfo->hp, &gScene->hierarchicalGameObjects.data()[1]->hp, sizeof(float));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbHPInfo->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(18, d3dGpuVirtualAddress);
}

void UI_HP_OtherPlayer::ReleaseShaderVariables()
{
	if (m_pd3dcbHPInfo)
	{
		m_pd3dcbHPInfo->Unmap(0, NULL);
		m_pd3dcbHPInfo->Release();
	}
}

D3D12_INPUT_LAYOUT_DESC UI_HP_OtherPlayer::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE UI_HP_OtherPlayer::CreateVertexShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSTextureUI_HP2", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_SHADER_BYTECODE UI_HP_OtherPlayer::CreatePixelShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSTextureUI_HP2", "ps_5_1", &m_pd3dPixelShaderBlob));
}

void UI_HP_OtherPlayer::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filePath, 0);
	Scene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 17, false);
}

UI_TimerBar::UI_TimerBar()
{
}

UI_TimerBar::UI_TimerBar(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	BuildObjects(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, filePath);
}

UI_TimerBar::~UI_TimerBar()
{
}

void UI_TimerBar::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(VS_CB_HP_INFO) + 255) & ~255); //256의 배수
	m_pd3dcbTimeInfo = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbTimeInfo->Map(0, NULL, (void**)&m_pcbMappedTimeInfo);
}

void UI_TimerBar::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	// Timer Info
	::memcpy(&m_pcbMappedTimeInfo->elapsedTime, &gScene->GameTimeElapsed, sizeof(float));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbTimeInfo->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(21, d3dGpuVirtualAddress);
}

void UI_TimerBar::ReleaseShaderVariables()
{
	if (m_pd3dcbTimeInfo)
	{
		m_pd3dcbTimeInfo->Unmap(0, NULL);
		m_pd3dcbTimeInfo->Release();
	}
}

D3D12_INPUT_LAYOUT_DESC UI_TimerBar::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE UI_TimerBar::CreateVertexShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSTextureUI_Timer", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_SHADER_BYTECODE UI_TimerBar::CreatePixelShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSTextureUI_Timer", "ps_5_1", &m_pd3dPixelShaderBlob));
}

void UI_TimerBar::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filePath, 0);
	Scene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 17, false);
}

UI_KeyInput_Right_Shift::UI_KeyInput_Right_Shift()
{
}

UI_KeyInput_Right_Shift::UI_KeyInput_Right_Shift(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	BuildObjects(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, filePath);
}

UI_KeyInput_Right_Shift::~UI_KeyInput_Right_Shift()
{
}

D3D12_INPUT_LAYOUT_DESC UI_KeyInput_Right_Shift::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE UI_KeyInput_Right_Shift::CreateVertexShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSTextureUI_KeyRightShift", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_SHADER_BYTECODE UI_KeyInput_Right_Shift::CreatePixelShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSTextureUI_KeyRightShift", "ps_5_1", &m_pd3dPixelShaderBlob));
}

void UI_KeyInput_Right_Shift::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filePath, 0);
	Scene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 17, false);
}

UI_KeyInput_Left_Shift::UI_KeyInput_Left_Shift()
{
}

UI_KeyInput_Left_Shift::UI_KeyInput_Left_Shift(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	BuildObjects(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, filePath);
}

UI_KeyInput_Left_Shift::~UI_KeyInput_Left_Shift()
{
}

D3D12_INPUT_LAYOUT_DESC UI_KeyInput_Left_Shift::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE UI_KeyInput_Left_Shift::CreateVertexShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSTextureUI_KeyLeftShift", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_SHADER_BYTECODE UI_KeyInput_Left_Shift::CreatePixelShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSTextureUI_KeyLeftShift", "ps_5_1", &m_pd3dPixelShaderBlob));
}

void UI_KeyInput_Left_Shift::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filePath, 0);
	Scene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 17, false);
}

UI_KeyInput_Space::UI_KeyInput_Space()
{
}

UI_KeyInput_Space::UI_KeyInput_Space(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	BuildObjects(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, filePath);
}

UI_KeyInput_Space::~UI_KeyInput_Space()
{
}

D3D12_INPUT_LAYOUT_DESC UI_KeyInput_Space::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE UI_KeyInput_Space::CreateVertexShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSTextureUI_KeySpace", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_SHADER_BYTECODE UI_KeyInput_Space::CreatePixelShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSTextureUI_KeySpace", "ps_5_1", &m_pd3dPixelShaderBlob));
}

void UI_KeyInput_Space::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filePath, 0);
	Scene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 17, false);
}

UI_PlayerTotalScore::UI_PlayerTotalScore()
{
}

UI_PlayerTotalScore::UI_PlayerTotalScore(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	BuildObjects(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, filePath);
}

UI_PlayerTotalScore::~UI_PlayerTotalScore()
{
}

void UI_PlayerTotalScore::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(VS_CB_HP_INFO) + 255) & ~255); //256의 배수
	m_pd3dcbScoreInfo = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbScoreInfo->Map(0, NULL, (void**)&m_pcbMappedScoreInfo);
}

void UI_PlayerTotalScore::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	// other player score info
	::memcpy(&m_pcbMappedScoreInfo->score, &gScene->m_pPlayer->score, sizeof(float));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbScoreInfo->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(20, d3dGpuVirtualAddress);
}

void UI_PlayerTotalScore::ReleaseShaderVariables()
{
	if (m_pd3dcbScoreInfo)
	{
		m_pd3dcbScoreInfo->Unmap(0, NULL);
		m_pd3dcbScoreInfo->Release();
	}
}

D3D12_INPUT_LAYOUT_DESC UI_PlayerTotalScore::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE UI_PlayerTotalScore::CreateVertexShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSTextureUI_PlayerTotalScore", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_SHADER_BYTECODE UI_PlayerTotalScore::CreatePixelShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSTextureUI_PlayerTotalScore", "ps_5_1", &m_pd3dPixelShaderBlob));
}

void UI_PlayerTotalScore::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filePath, 0);
	Scene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 19, false);
}

UI_OtherPlayerTotalScore::UI_OtherPlayerTotalScore()
{
}

UI_OtherPlayerTotalScore::UI_OtherPlayerTotalScore(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	BuildObjects(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, filePath);
}

UI_OtherPlayerTotalScore::~UI_OtherPlayerTotalScore()
{
}

void UI_OtherPlayerTotalScore::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(VS_CB_HP_INFO) + 255) & ~255); //256의 배수
	m_pd3dcbScoreInfo = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbScoreInfo->Map(0, NULL, (void**)&m_pcbMappedScoreInfo);
}

void UI_OtherPlayerTotalScore::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	// other player score info
	::memcpy(&m_pcbMappedScoreInfo->score, &gScene->hierarchicalGameObjects.data()[1]->score, sizeof(float));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbScoreInfo->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(20, d3dGpuVirtualAddress);
}

void UI_OtherPlayerTotalScore::ReleaseShaderVariables()
{
	if (m_pd3dcbScoreInfo)
	{
		m_pd3dcbScoreInfo->Unmap(0, NULL);
		m_pd3dcbScoreInfo->Release();
	}
}

D3D12_INPUT_LAYOUT_DESC UI_OtherPlayerTotalScore::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE UI_OtherPlayerTotalScore::CreateVertexShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSTextureUI_OtherPlayerTotalScore", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_SHADER_BYTECODE UI_OtherPlayerTotalScore::CreatePixelShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSTextureUI_OtherPlayerTotalScore", "ps_5_1", &m_pd3dPixelShaderBlob));
}

void UI_OtherPlayerTotalScore::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filePath, 0);
	Scene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 19, false);
}

UI_BloodEffect::UI_BloodEffect()
{
}

UI_BloodEffect::UI_BloodEffect(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	BuildObjects(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, filePath);
}

UI_BloodEffect::~UI_BloodEffect()
{
}

void UI_BloodEffect::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void UI_BloodEffect::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void UI_BloodEffect::ReleaseShaderVariables()
{
}

D3D12_INPUT_LAYOUT_DESC UI_BloodEffect::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_BLEND_DESC UI_BloodEffect::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = TRUE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);
}

D3D12_SHADER_BYTECODE UI_BloodEffect::CreateVertexShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSTextureUI_BloodEffect", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_SHADER_BYTECODE UI_BloodEffect::CreatePixelShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSTextureUI_BloodEffect", "ps_5_1", &m_pd3dPixelShaderBlob));
}

void UI_BloodEffect::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filePath, 0);
	Scene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 17, false);
}

UI_ReadyFight::UI_ReadyFight()
{
}

UI_ReadyFight::UI_ReadyFight(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	BuildObjects(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, filePath);
}

UI_ReadyFight::~UI_ReadyFight()
{
}

void UI_ReadyFight::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void UI_ReadyFight::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void UI_ReadyFight::ReleaseShaderVariables()
{
}

D3D12_INPUT_LAYOUT_DESC UI_ReadyFight::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_BLEND_DESC UI_ReadyFight::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = TRUE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);
}

D3D12_SHADER_BYTECODE UI_ReadyFight::CreateVertexShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSTextureUI_ReadyFight", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_SHADER_BYTECODE UI_ReadyFight::CreatePixelShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSTextureUI_ReadyFight", "ps_5_1", &m_pd3dPixelShaderBlob));
}

void UI_ReadyFight::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filePath, 0, false);
	Scene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 17, false);
}

UI_LOSEWIN::UI_LOSEWIN()
{
}

UI_LOSEWIN::UI_LOSEWIN(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	BuildObjects(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, filePath);
}

UI_LOSEWIN::~UI_LOSEWIN()
{
}

void UI_LOSEWIN::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void UI_LOSEWIN::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void UI_LOSEWIN::ReleaseShaderVariables()
{
}

D3D12_INPUT_LAYOUT_DESC UI_LOSEWIN::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_BLEND_DESC UI_LOSEWIN::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = TRUE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);
}

D3D12_SHADER_BYTECODE UI_LOSEWIN::CreateVertexShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSTextureUI_LoseWin", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_SHADER_BYTECODE UI_LOSEWIN::CreatePixelShader()
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSTextureUI_LoseWin", "ps_5_1", &m_pd3dPixelShaderBlob));
}

void UI_LOSEWIN::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wchar_t* filePath)
{
	CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filePath, 0);
	Scene::CreateShaderResourceViews(pd3dDevice, m_pTexture, 17, false);
}
