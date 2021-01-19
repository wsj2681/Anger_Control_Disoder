#include "framework.h"
#include "Scene.h"
#include "Camera.h"

void Scene::BuildObjects(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, FbxManager* pfbxSdkManager, FbxScene* pfbxScene)
{
	this->graphicsRootSignature = CreateGraphicsRootSignature(device);

	//Build Objects

	CreateShaderVariables(device, commandList);
}

void Scene::ReleaseObjects()
{
	SAFE_RELEASE(graphicsRootSignature);

	ReleaseShaderVariables();
}

void Scene::CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
}

void Scene::UpdateShaderVariables(ID3D12GraphicsCommandList* commandList)
{
}

void Scene::ReleaseShaderVariables()
{
}

ID3D12RootSignature* Scene::CreateGraphicsRootSignature(ID3D12Device* device)
{
	ID3D12RootSignature* graphicsRootSignature = nullptr;

	D3D12_ROOT_PARAMETER rootParameters[2];


/*	TODO : Create Object and Shader
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = 
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | 
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | 
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | 
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	::ZeroMemory(&rootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	rootSignatureDesc.NumParameters = _countof(rootParameters);
	rootSignatureDesc.pParameters = rootParameters;
	rootSignatureDesc.NumStaticSamplers = 0;
	rootSignatureDesc.pStaticSamplers = NULL;
	rootSignatureDesc.Flags = rootSignatureFlags;
*/
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	//TODO : 
	//D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	//pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&pd3dGraphicsRootSignature);

	SAFE_RELEASE(signatureBlob);
	SAFE_RELEASE(errorBlob);

	return graphicsRootSignature;
}

ID3D12RootSignature* Scene::GetGraphicsRootSignature()
{
	return this->graphicsRootSignature;
}

void Scene::AnimateObjects(float eTime)
{
}

void Scene::Render(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	if (graphicsRootSignature) commandList->SetGraphicsRootSignature(graphicsRootSignature);


	UpdateShaderVariables(commandList);

	//Render

}
