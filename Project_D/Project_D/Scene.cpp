#include "framework.h"
#include "Scene.h"
#include "Camera.h"

void Scene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{

}

void Scene::ReleaseObjects()
{

}

ID3D12RootSignature* Scene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
	// Texture : RANGE_TYPE_SRV
	// Object : RANGE_TYPE_CBV
	
	
	// DescriptorRange�� �����մϴ�.
	// DescriptorRange�� Ÿ�԰� ���̴����Ͽ� �� �������� ��ȣ�� �����ϴ�.
	// ���������� �ڷ����� �迭�̶�� NumDescriptors�� ������ �迭�� ������ ��ġ ��ŵ�ϴ�.

	ID3D12RootSignature* graphicsRootsignature = nullptr;

	D3D12_DESCRIPTOR_RANGE descriptorRanges[2];
	
	descriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descriptorRanges[0].NumDescriptors = 1;
	descriptorRanges[0].BaseShaderRegister = 0; //Shader register Number
	descriptorRanges[0].RegisterSpace = 0;
	descriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRanges[1].NumDescriptors = 1;
	descriptorRanges[1].BaseShaderRegister = 1; //Shader register Number
	descriptorRanges[1].RegisterSpace = 0;
	descriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;



	// Texture : D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE
	// Object : D3D12_ROOT_PARAMETER_TYPE_CBV


	// ��Ʈ �Ķ���͸� �����մϴ�.
	// �Ķ������ Ÿ���� ������ ���� �����մϴ�.
	// ���� ���̴��� �������Ϳ� �ִ� �����͸� ����Ѵٸ�
	// DescriptorTable�� ����Ͽ� NumDescriptorRanges�� pDescriptorRanges�� �����մϴ�.
	// �׸��� ���̴����� ������ �κ��� ���մϴ�.

	D3D12_ROOT_PARAMETER rootParameters[2];

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[0].Descriptor.RegisterSpace = 0;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParameters[1].DescriptorTable.pDescriptorRanges = &descriptorRanges[1];
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// Sampler���¸� �����մϴ�.

	D3D12_STATIC_SAMPLER_DESC samplerDescs[2];

	samplerDescs[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescs[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDescs[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDescs[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDescs[0].MipLODBias = 0;
	samplerDescs[0].MaxAnisotropy = 1;
	samplerDescs[0].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	samplerDescs[0].MinLOD = 0;
	samplerDescs[0].MaxLOD = D3D12_FLOAT32_MAX;
	samplerDescs[0].ShaderRegister = 0;
	samplerDescs[0].RegisterSpace = 0;
	samplerDescs[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	samplerDescs[1].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescs[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDescs[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDescs[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDescs[1].MipLODBias = 0;
	samplerDescs[1].MaxAnisotropy = 1;
	samplerDescs[1].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	samplerDescs[1].MinLOD = 0;
	samplerDescs[1].MaxLOD = D3D12_FLOAT32_MAX;
	samplerDescs[1].ShaderRegister = 1;
	samplerDescs[1].RegisterSpace = 0;
	samplerDescs[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//��Ʈ �ñ״��ĸ� �����մϴ�.
	//������ ���� ��Ʈ �Ķ���Ϳ� ���÷� ���¸� �־��ݴϴ�.

	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	::ZeroMemory(&rootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	rootSignatureDesc.NumParameters = _countof(rootParameters);
	rootSignatureDesc.pParameters = rootParameters;
	rootSignatureDesc.NumStaticSamplers = _countof(samplerDescs);
	rootSignatureDesc.pStaticSamplers = samplerDescs;
	rootSignatureDesc.Flags = rootSignatureFlags;

	ID3DBlob *signatureBlob = nullptr;
	ID3DBlob *errorBlob = nullptr;
	HR(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob));
	HR(pd3dDevice->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(),__uuidof(ID3D12RootSignature), (void**)&graphicsRootSignature));

	SAFE_RELEASE(signatureBlob);
	SAFE_RELEASE(errorBlob);

	return graphicsRootSignature;
}

ID3D12RootSignature* Scene::GetGraphicsRootSignature()
{
	return nullptr;
}

void Scene::SetGraphicsRootSignature(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void Scene::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void Scene::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void Scene::ReleaseShaderVariables()
{
}

void Scene::Animate(float etime)
{
}

void Scene::OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	pd3dCommandList->SetGraphicsRootSignature(this->graphicsRootSignature);

	UpdateShaderVariables(pd3dCommandList);

	//TODO : Lights Set GraphicsRootConstantBufferView
}

void Scene::OnPreRender(ID3D12Device* pd3dDevice, ID3D12CommandQueue* pd3dCommandQueue, ID3D12Fence* pd3dFence, HANDLE hFenceEvent)
{
}

void Scene::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	//TODO : SetViewportsAndScissorRects �Լ� ����
	//pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	//TODO : Render
}
