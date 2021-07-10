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
	::memcpy(&m_pcbMappedHPInfo->hp, &gScene->hierarchicalGameObjects.data()[0]->hp, sizeof(float));

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
	::memcpy(&m_pcbMappedScoreInfo->score, &gScene->hierarchicalGameObjects.data()[1]->score, sizeof(float));

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