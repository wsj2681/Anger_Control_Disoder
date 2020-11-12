#include "framework.h"
#include "Scene.h"

void Scene::BulidObjects(ID3D12Device* Device, ID3D12GraphicsCommandList* CommandList)
{
	this->GraphicRootSignature = this->CreateGraphicsRootSignature(Device);

	this->soundmanager = new SoundManager("Sound/sound2.mp3", false);
	this->soundmanager->Play();
	cout << "SoundManager Build OK" << endl;

	this->shader = new Shader();
	shader->CreateShader(Device, this->GraphicRootSignature, D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
}

void Scene::ReleaseObjects()
{
	if (this->GraphicRootSignature)
		this->GraphicRootSignature->Release();

	if (this->soundmanager)
	{
		this->soundmanager->Release();
		delete this->soundmanager;
	}

	if (this->shader)
	{
		this->shader->Release();
	}

	// Object->Release();
	// delete Object();
}

ID3D12RootSignature* Scene::CreateGraphicsRootSignature(ID3D12Device* Device)
{
	// 서술자 힙 서술자입니다.
	// 필요한 크기만큼 늘려 인덱스마다 서술해줍니다.
	D3D12_DESCRIPTOR_RANGE DescriptorRanges[2];

	DescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV; // 상수 버퍼 뷰 입니다.
	DescriptorRanges[0].NumDescriptors = 1;
	DescriptorRanges[0].BaseShaderRegister = 1;// 쉐이더 레지스터 번호입니다.
	DescriptorRanges[0].RegisterSpace = 0;
	DescriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	DescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //쉐이더 리소스 뷰입니다.
	DescriptorRanges[1].NumDescriptors = 1;
	DescriptorRanges[1].BaseShaderRegister = 0; //t0: gtxtTexture
	DescriptorRanges[1].RegisterSpace = 0;
	DescriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	// 루트 파라미터 입니다.
	// 쉐이더레지스터에 적용할 레지스터 번호를 작성합니다.
	D3D12_ROOT_PARAMETER RootParameters[1];
	RootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	RootParameters[0].Descriptor.ShaderRegister = 0; //Player
	RootParameters[0].Descriptor.RegisterSpace = 0;
	RootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	// 샘플러 상태 서술자 입니다.
	// 샘플러 상태 갯수만큼 배열을 늘려 인덱스마다 서술자를 만들어줍니다.
	D3D12_STATIC_SAMPLER_DESC SamplerDescs[1];
	SamplerDescs[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDescs[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	SamplerDescs[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	SamplerDescs[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	SamplerDescs[0].MipLODBias = 0;
	SamplerDescs[0].MaxAnisotropy = 1;
	SamplerDescs[0].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	SamplerDescs[0].MinLOD = 0;
	SamplerDescs[0].MaxLOD = D3D12_FLOAT32_MAX;
	SamplerDescs[0].ShaderRegister = 0;	// 쉐이더 레지스터입니다. 쉐이더 레지스터 번호를 정해줍니다.
	SamplerDescs[0].RegisterSpace = 0;
	SamplerDescs[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;



	D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc;
	::ZeroMemory(&RootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	RootSignatureDesc.NumParameters = 0;		// RootParameters count
	RootSignatureDesc.pParameters = nullptr;	// RootParameters Pointer
	RootSignatureDesc.NumStaticSamplers = 0;	// SamplerDescs count
	RootSignatureDesc.pStaticSamplers = nullptr;// SamplerDescs Pointer
	RootSignatureDesc.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
		| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
		| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;

	ID3D12RootSignature* GraphicsRootSignature = nullptr;

	ID3DBlob* SignatureBlob = nullptr;
	ID3DBlob* ErrorBlob = nullptr;

	D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &SignatureBlob, &ErrorBlob);
	Device->CreateRootSignature(0, SignatureBlob->GetBufferPointer(), SignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&GraphicsRootSignature);

	if (SignatureBlob)
		SignatureBlob->Release();
	if (ErrorBlob)
		ErrorBlob->Release();

	return GraphicsRootSignature;
}

ID3D12RootSignature* Scene::GetGraphicsRootSignature()
{
	return this->GraphicRootSignature;
}

void Scene::SetGraphicRootSignature(ID3D12GraphicsCommandList* CommandList)
{
	CommandList->SetGraphicsRootSignature(this->GraphicRootSignature);
}

void Scene::AnimateObjects(float fDeltaTime)
{
	//Object->Animate(fDeltaTime);
}

void Scene::PreRender(ID3D12GraphicsCommandList * CommandList, Camera * camera)
{
	CommandList->SetGraphicsRootSignature(this->GraphicRootSignature);

}

void Scene::Render(ID3D12GraphicsCommandList* CommandList)
{
	this->soundmanager->Update();
	this->shader->UpdateShader(CommandList);
	//Object->Render();
}

void Scene::ReleaseUploadBuffers()
{
	//shader->releaseUploadBuffers();
	//Object->releaseUploadBuffers();
}

void Scene::OnProcessingMouseMessage(HWND hWnd, UINT MessageID, WPARAM wParam, LPARAM lParam)
{
}

void Scene::OnProcessingKeyboardMessage(HWND hWnd, UINT MessageID, WPARAM wParam, LPARAM lParam)
{
	switch (MessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		case VK_RETURN:
			break;
		case VK_F1:
			this->soundmanager->Pause();
			break;
		case VK_F2:
			this->soundmanager->Resume();
			break;
		case VK_PRIOR:
			this->soundmanager->VolumeUp();
			break;
		case VK_NEXT:
			this->soundmanager->VolumeDown();
		default:
			break;
		}
		break;
	default:
		break;
	}
}

LRESULT Scene::OnProecssingWindowMessage(HWND hWnd, UINT MessageID, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}