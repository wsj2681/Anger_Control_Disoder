#include "framework.h"
#include "Scene.h"


void Scene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	graphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);
	
	objects = new Object*[1];
	Shader* shader = new CubeShader();
	Mesh* mesh = new CubeMesh(pd3dDevice, pd3dCommandList);

	shader->CreateShader(pd3dDevice, pd3dCommandList, graphicsRootSignature);

	objects[0]->SetMesh(mesh);
	objects[0]->SetShader(shader);
}

void Scene::ReleaseObjects()
{
}

ID3D12RootSignature* Scene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
	ID3D12RootSignature* rootSignature = nullptr;

	D3D12_DESCRIPTOR_RANGE descRanges[1];
	descRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descRanges[0].NumDescriptors = 1;
	descRanges[0].BaseShaderRegister = 2; //GameObject
	descRanges[0].RegisterSpace = 0;
	descRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParameter[2];
	rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[0].Descriptor.ShaderRegister = 1; //Camera
	rootParameter[0].Descriptor.RegisterSpace = 0;
	rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParameter[1].DescriptorTable.pDescriptorRanges = &descRanges[0];
	rootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc;
	::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.MipLODBias = 0;
	d3dSamplerDesc.MaxAnisotropy = 1;
	d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc.ShaderRegister = 0;
	d3dSamplerDesc.RegisterSpace = 0;
	d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(rootParameter);
	d3dRootSignatureDesc.pParameters = rootParameter;
	d3dRootSignatureDesc.NumStaticSamplers = 1;
	d3dRootSignatureDesc.pStaticSamplers = &d3dSamplerDesc;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob* pd3dSignatureBlob = NULL;
	ID3DBlob* pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&rootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return rootSignature;
}

void Scene::CheckCollision()
{
}

void Scene::Animate()
{
}

void Scene::Render()
{
	
}
