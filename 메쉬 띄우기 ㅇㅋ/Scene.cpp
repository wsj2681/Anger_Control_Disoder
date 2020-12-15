#include "Scene.h"

CScene::CScene() {

}
CScene::~CScene() {

}

void CScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) {
	//그래픽 루트 시그너쳐를 생성한다.
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);
	
	MapMesh* pCubeMesh = new MapMesh(pd3dDevice, pd3dCommandList);
	m_nObjects = 1;
	m_ppObjects = new CGameObject * [m_nObjects];
	
	MapObject* pRotatingObject = new MapObject();
	pRotatingObject->SetMesh(pCubeMesh);

	CDiffusedShader* pShader = new CDiffusedShader();
	pShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	pShader->BuildObjects(pd3dDevice, pd3dCommandList, NULL);
	
	pRotatingObject->SetShader(pShader);
	
	m_ppObjects[0] = pRotatingObject;
}

void CScene::ReleaseObjects() {
	if (m_pd3dGraphicsRootSignature)
		m_pd3dGraphicsRootSignature->Release();

	if (m_ppObjects){
		for (int i = 0; i < m_nObjects; i++)
			if (m_ppObjects[i])
				delete m_ppObjects[i];

		delete[] m_ppObjects;
	}
}

void CScene::ReleaseUploadBuffers() {
	if (m_ppObjects){
		for (int i = 0; i < m_nObjects; i++)
			if (m_ppObjects[i])
				m_ppObjects[i]->ReleaseUploadBuffers();
	}
}

ID3D12RootSignature* CScene::GetGraphicsRootSignature() {
	return(m_pd3dGraphicsRootSignature);
}

ID3D12RootSignature* CScene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice){
	
	ID3D12RootSignature* pd3dGraphicsRootSignature = NULL;
	
	D3D12_ROOT_PARAMETER pd3dRootParameters[2];	
	
	pd3dRootParameters[0].ParameterType				= D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[0].Constants.Num32BitValues	= 16;
	pd3dRootParameters[0].Constants.ShaderRegister	= 0;
	pd3dRootParameters[0].Constants.RegisterSpace	= 0;
	pd3dRootParameters[0].ShaderVisibility			= D3D12_SHADER_VISIBILITY_VERTEX;
	
	pd3dRootParameters[1].ParameterType				= D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues	= 32;
	pd3dRootParameters[1].Constants.ShaderRegister	= 1;
	pd3dRootParameters[1].Constants.RegisterSpace	= 0;
	pd3dRootParameters[1].ShaderVisibility			= D3D12_SHADER_VISIBILITY_VERTEX;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
	
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc = {};
	
	d3dRootSignatureDesc.NumParameters		= _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters		= pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers	= 0;
	d3dRootSignatureDesc.pStaticSamplers	= NULL;
	d3dRootSignatureDesc.Flags				= d3dRootSignatureFlags;
	
	ID3DBlob* pd3dSignatureBlob = NULL;
	ID3DBlob* pd3dErrorBlob = NULL;
	
	::D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1,&pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(),pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&pd3dGraphicsRootSignature);
	
	if (pd3dSignatureBlob)
		pd3dSignatureBlob->Release();
	if (pd3dErrorBlob)
		pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam){
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CScene::ProcessInput(){
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed){
	for (int i = 0; i < m_nObjects; i++)
		m_ppObjects[i]->Animate(fTimeElapsed);
}

void CScene::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera) {
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	
	if (pCamera)
		pCamera->UpdateShaderVariables(pd3dCommandList);
	//씬을 렌더링하는 것은 씬을 구성하는 게임 객체(셰이더를 포함하는 객체)들을 렌더링하는 것이다.
	for (int i = 0; i < m_nObjects; i++)
	{
		if (m_ppObjects[i]) m_ppObjects[i]->Render(pd3dCommandList, pCamera);
	}
}