//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "Scene.h"
#include "Texture.h"
#include "ModelInfo.h"
#include "AnimationController.h"
#include "AnimationSet.h"
#include "AnimationTrack.h"
#include "Material.h"
#include "CallBack.h"
#include "AnimationSet.h"
#include "SkyBox.h"
#include "BoxerObject.h"
#include "CrowdObject.h"
#include "Timer.h"
#include "UIShader.h"
#include "CubeObject.h"

ID3D12DescriptorHeap *Scene::m_pd3dCbvSrvDescriptorHeap = nullptr;

D3D12_CPU_DESCRIPTOR_HANDLE	Scene::m_d3dCbvCPUDescriptorStartHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	Scene::m_d3dCbvGPUDescriptorStartHandle;
D3D12_CPU_DESCRIPTOR_HANDLE	Scene::m_d3dSrvCPUDescriptorStartHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	Scene::m_d3dSrvGPUDescriptorStartHandle;

D3D12_CPU_DESCRIPTOR_HANDLE	Scene::m_d3dCbvCPUDescriptorNextHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	Scene::m_d3dCbvGPUDescriptorNextHandle;
D3D12_CPU_DESCRIPTOR_HANDLE	Scene::m_d3dSrvCPUDescriptorNextHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	Scene::m_d3dSrvGPUDescriptorNextHandle;

#define OTHERPLAYER 1
#define CUBEOBJECT 2
#define SPHEHROBJECT 3

vector<Object*> gGameObject{};

//std::random_device rd{};
//std::default_random_engine dre{ rd() };
//uniform_int_distribution<> uid{ ANIMATION_MOVE_FORWARD, ANIMATION_GUARD_RIGHT_HEAD }; TODO: 이동 처리
//std::uniform_int_distribution<> uid{ ANIMATION_HOOK_L, ANIMATION_GUARD_RIGHT_HEAD };

UINT aniNum{ ANIMATION_HOOK_L };

GameTimer countTimer{};

Scene::Scene()
{
	SoundManager::Init();

	soundManager = new SoundManager("Sound/ACD_mini.mid", true);
	
	hitSound = new SoundManager("Sound/Hit01.mp3");
	attackSound = new SoundManager("Sound/Punch01.mp3");

	soundManager->Play();
}

Scene::~Scene()
{
	SoundManager::Release();
}

void Scene::BuildDefaultLightsAndMaterials()
{
	/*
	0 ~ 37 = Roof Light
	38~39 = RedSpotLight
	40~42 = HallWayLight
	43 = CollideLight
	*/
	m_nLights = lightsCount;
	m_nLights += 4;
	m_pLights = new LIGHT[m_nLights];
	::ZeroMemory(m_pLights, sizeof(LIGHT) * m_nLights);

	m_xmf4GlobalAmbient = XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	for (int i = 0; i < 38; ++i)
	{
		if (i % 2)
		{
			m_pLights[i].m_bEnable = true;
		}
		else
		{
			m_pLights[i].m_bEnable = false;
		}
		m_pLights[i].m_nType = DIRECTIONAL_LIGHT;
		m_pLights[i].m_fRange = 120.0f;
		m_pLights[i].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		m_pLights[i].m_xmf4Diffuse = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		m_pLights[i].m_xmf4Specular = XMFLOAT4(0.01f, 0.01f, 0.01f, 0.0f);
		m_pLights[i].m_xmf3Position = lights.data()[i]->GetPosition();
		m_pLights[i].m_xmf3Direction = XMFLOAT3(0.0f, -.5f, 0.0f);
	}

	for (int i = 38; i < 40; ++i)
	{
		m_pLights[i].m_bEnable = true;
		m_pLights[i].m_nType = SPOT_LIGHT;
		m_pLights[i].m_fRange = 150.0f;
		m_pLights[i].m_xmf4Ambient = XMFLOAT4(0.9f, 0.f, 0.f, 1.0f);
		m_pLights[i].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.f, 0.f, 1.0f);
		m_pLights[i].m_xmf4Specular = XMFLOAT4(0.7f, 0.f, 0.f, 0.0f);
		m_pLights[i].m_xmf3Position = lights.data()[i]->GetPosition();
		//m_pLights[i].m_xmf3Direction = XMFLOAT3(0.f, 60.f, -252.6f);
		m_pLights[i].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
		m_pLights[i].m_fFalloff = 100.0f;
		m_pLights[i].m_fPhi = (float)cos(XMConvertToRadians(60.0f));
		m_pLights[i].m_fTheta = (float)cos(XMConvertToRadians(20.0f));
	}
	m_pLights[40].m_bEnable = false;
	m_pLights[40].m_nType = SPOT_LIGHT;
	m_pLights[40].m_fRange = 400.0f;
	m_pLights[40].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.f, 1.0f);
	m_pLights[40].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.f, 1.0f);
	m_pLights[40].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.f, 0.0f);
	m_pLights[40].m_xmf3Position = XMFLOAT3(0.f, 100.7337f, -750.f);
	m_pLights[40].m_xmf3Direction = XMFLOAT3(0.f, -1.f, 0.f);
	m_pLights[40].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[40].m_fFalloff = 100.0f;
	m_pLights[40].m_fPhi = (float)cos(XMConvertToRadians(60.0f));
	m_pLights[40].m_fTheta = (float)cos(XMConvertToRadians(20.0f));

	m_pLights[41].m_bEnable = false;
	m_pLights[41].m_nType = SPOT_LIGHT;
	m_pLights[41].m_fRange = 400.0f;
	m_pLights[41].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.f, 1.0f);
	m_pLights[41].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.f, 1.0f);
	m_pLights[41].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.f, 0.0f);
	m_pLights[41].m_xmf3Position = XMFLOAT3(0.f, 100.7337f, -675.f);
	m_pLights[41].m_xmf3Direction = XMFLOAT3(0.f, -1.f, 0.f);
	m_pLights[41].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[41].m_fFalloff = 50.0f;
	m_pLights[41].m_fPhi = (float)cos(XMConvertToRadians(60.0f));
	m_pLights[41].m_fTheta = (float)cos(XMConvertToRadians(20.0f));

	m_pLights[42].m_bEnable = false;
	m_pLights[42].m_nType = SPOT_LIGHT;
	m_pLights[42].m_fRange = 400.0f;
	m_pLights[42].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.f, 1.0f);
	m_pLights[42].m_xmf4Diffuse = XMFLOAT4(0.1f, 0.1f, 0.f, 1.0f);
	m_pLights[42].m_xmf4Specular = XMFLOAT4(0.1f, 0.1f, 0.f, 0.0f);
	m_pLights[42].m_xmf3Position = XMFLOAT3(0.f, 100.7337f, -550.f);
	m_pLights[42].m_xmf3Direction = XMFLOAT3(0.f, -1.f, 0.f);
	m_pLights[42].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[42].m_fFalloff = 50.0f;
	m_pLights[42].m_fPhi = (float)cos(XMConvertToRadians(60.0f));
	m_pLights[42].m_fTheta = (float)cos(XMConvertToRadians(20.0f));

	// Collide Light
	m_pLights[43].m_bEnable = false;
	m_pLights[43].m_nType = SPOT_LIGHT;
	m_pLights[43].m_fRange = 100.0f;
	m_pLights[43].m_xmf4Ambient = XMFLOAT4(0.1f, 0.0f, 0.0f, .3f);
	m_pLights[43].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.0f, 0.0f, .3f);
	m_pLights[43].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[43].m_xmf3Position = XMFLOAT3(0.f, 100.7337f, -550.f);
	m_pLights[43].m_xmf3Direction = XMFLOAT3(0.f, -1.f, 0.f);
	m_pLights[43].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[43].m_fFalloff = 8.0f;
	m_pLights[43].m_fPhi = (float)cos(XMConvertToRadians(90.0f));
	m_pLights[42].m_fTheta = (float)cos(XMConvertToRadians(30.0f));
}

void Scene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);


	CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 76); //SuperCobra(17), Gunship(2), Player:Mi24(1), Angrybot()

	Material::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	
	m_pSkyBox = new SkyBox(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);

	ModelInfo* MapModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Arena.bin", nullptr);
	Object* Map = new CrowdObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, MapModel, 1);
	cageSide = Map->FindFrame("octagon_floor");
	Map->SetPosition(0.0f, 0.f, 0.0f);
	cageCollision.Center = XMFLOAT3(0.f, 10.f, 0.f);
	cageCollision.Radius = 60.f;
	hierarchicalGameObjects.push_back(Map);
	SAFE_DELETE(MapModel);

	//조명 벡터 만들었다.
	lightsCount = 38;
	
	char name[30];
	lights.push_back(Map->FindFrame("light"));
	for (int i = 1; i < lightsCount; ++i)
	{
		sprintf(name, "light%d", i);
		lights.push_back(Map->FindFrame(name));
	}
	lightsCount += 2;
	lights.push_back(Map->FindFrame("spot_light"));
	lights.push_back(Map->FindFrame("spot_light_1"));

	BuildDefaultLightsAndMaterials();
	ModelInfo* BoxerModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/ThaiBoxer.bin", nullptr);
	Object* boxer = new BoxerObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, BoxerModel, 1);
	boxer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_COMBAT_MODE_A);
	for (int i = 0; i < boxer->m_pSkinnedAnimationController->m_pAnimationSets->m_nAnimationSets; ++i)
		boxer->m_pSkinnedAnimationController->m_pAnimationSets->m_pAnimationSets[i]->isOtherPlayer = true;
	boxer->SetPosition(-1.0f, 8.5f, 30.0f);
	boxer->Rotate(0.0f, 180.0f, 0.0f);

	hierarchicalGameObjects.push_back(boxer);
	SAFE_DELETE(BoxerModel);

	ModelInfo* cubeModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Cube.bin", nullptr);
	Object* cube = new CrowdObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, cubeModel, 1);
	cube->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
	cube->isActive = false;
	cube->SetScale(1.5f, 1.5f, 1.5f);
	hierarchicalGameObjects.push_back(cube);

	ModelInfo* sphereModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/sphere.bin", nullptr);
	Object* sphere = new CrowdObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, sphereModel, 1);
	sphere->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
	sphere->isActive = false;
	sphere->SetScale(1.5f, 1.5f, 1.5f);
	hierarchicalGameObjects.push_back(sphere);

	SAFE_DELETE(cubeModel);

	ModelInfo* crowdModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Crowd.bin", nullptr);

	int nFloors = 4;
	size_t nBaseModels = hierarchicalGameObjects.size();
	int nCrowds = 9;

	float angle = 0.0f;
	float radius = 130.0f;

	//TODO: 관중 각도 조절
	for (int i = 0; i < nFloors; ++i)
	{
		angle = -30.0f;
		for (int j = (int)nBaseModels + i * nCrowds; j < (int)nBaseModels + (i + 1) * nCrowds; ++j)
		{
			Object* crowd = new CrowdObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, crowdModel, 1);
			crowd->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 1);
			crowd->SetPosition(cos(XMConvertToRadians(angle)) * radius, -9.0f + 4.0f * i, sin(XMConvertToRadians(angle)) * radius);
			//crowd->Rotate(0.0f, angle + 90.f + ((j - nBaseModels) % (nCrowds - 1)) * 30.0f, 0.0f);
			crowd->Rotate(0.0f, 180.0f, 0.0f);
			hierarchicalGameObjects.push_back(crowd);
			angle += 30.f;
		}
		radius += 25.0f;
	}
	
	SAFE_DELETE(crowdModel);

	gGameObject = hierarchicalGameObjects;
	
	ui["PlayerHP"] = new UI_HP_Player(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/DDSfile/HPBar_Other.dds");
	ui["OtherPlayerHP"] = new UI_HP_OtherPlayer(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/DDSfile/HPBar.dds");
	ui["OtherPlayerHP"]->SetActive(true);
	//ui["Right_Shift_Black"] = new UI_KeyInput_Right_Shift(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/DDSfile/Key_Left_Shift.dds");
	//ui["Right_Shift_Red"] = new UI_KeyInput_Left_Shift(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/DDSfile/Key_Right_Shift.dds");
	//ui["Space"] = new UI_KeyInput_Space(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/DDSfile/Key_Space.dds");

	ui["0_PlayerTotalScore"] = new UI_PlayerTotalScore(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/DDSfile/Points_Empty.dds");
	ui["1_PlayerTotalScore"] = new UI_PlayerTotalScore(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/DDSfile/Points_L1.dds");
	ui["2_PlayerTotalScore"] = new UI_PlayerTotalScore(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/DDSfile/Points_L2.dds");
	ui["3_PlayerTotalScore"] = new UI_PlayerTotalScore(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/DDSfile/Points_Full.dds");
	ui["0_PlayerTotalScore"]->SetActive(true);
	ui["1_PlayerTotalScore"]->SetActive(false);
	ui["2_PlayerTotalScore"]->SetActive(false);
	ui["3_PlayerTotalScore"]->SetActive(false);

	// ui 하나로 바꿔서 넣어야할 듯
	//ui["0_OtherPlayerTotalScore"] = new UI_OtherPlayerTotalScore(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/DDSfile/Points_Empty.dds");
	//ui["1_OtherPlayerTotalScore"] = new UI_OtherPlayerTotalScore(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/DDSfile/Points_R1.dds");
	//ui["2_OtherPlayerTotalScore"] = new UI_OtherPlayerTotalScore(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/DDSfile/Points_R2.dds");
	//ui["3_OtherPlayerTotalScore"] = new UI_OtherPlayerTotalScore(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/DDSfile/Points_Full.dds");
	//ui["0_OtherPlayerTotalScore"]->SetActive(true);
	//ui["1_OtherPlayerTotalScore"]->SetActive(false);
	//ui["2_OtherPlayerTotalScore"]->SetActive(false);
	//ui["3_OtherPlayerTotalScore"]->SetActive(false);

	particle = new Particle;
	particle->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void Scene::Release()
{
	SAFE_RELEASE(m_pd3dGraphicsRootSignature);
	SAFE_RELEASE(m_pd3dCbvSrvDescriptorHeap);

	if (m_ppGameObjects)
	{
		for (int i = 0; i < m_nGameObjects; i++) SAFE_RELEASE(m_ppGameObjects[i]);
		delete[] m_ppGameObjects;
	}

	if (m_ppShaders)
	{
		for (int i = 0; i < m_nShaders; i++)
		{
			m_ppShaders[i]->ReleaseShaderVariables();
			m_ppShaders[i]->ReleaseObjects();
			m_ppShaders[i]->Release();
		}
		delete[] m_ppShaders;
	}

	SAFE_DELETE(m_pSkyBox);

	if (!hierarchicalGameObjects.empty())
	{
		for (auto& obj : hierarchicalGameObjects)
		{
			SAFE_RELEASE(obj);
		}
		hierarchicalGameObjects.clear();
	}

	ReleaseShaderVariables();

	SAFE_DELETEARR(m_pLights);
}

ID3D12RootSignature *Scene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	ID3D12RootSignature *pd3dGraphicsRootSignature = nullptr;

	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[13];

	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors = 1;
	pd3dDescriptorRanges[0].BaseShaderRegister = 6; //t6: gtxtAlbedoTexture
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[1].NumDescriptors = 1;
	pd3dDescriptorRanges[1].BaseShaderRegister = 7; //t7: gtxtSpecularTexture
	pd3dDescriptorRanges[1].RegisterSpace = 0;
	pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[2].NumDescriptors = 1;
	pd3dDescriptorRanges[2].BaseShaderRegister = 8; //t8: gtxtNormalTexture
	pd3dDescriptorRanges[2].RegisterSpace = 0;
	pd3dDescriptorRanges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[3].NumDescriptors = 1;
	pd3dDescriptorRanges[3].BaseShaderRegister = 9; //t9: gtxtMetallicTexture
	pd3dDescriptorRanges[3].RegisterSpace = 0;
	pd3dDescriptorRanges[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[4].NumDescriptors = 1;
	pd3dDescriptorRanges[4].BaseShaderRegister = 10; //t10: gtxtEmissionTexture
	pd3dDescriptorRanges[4].RegisterSpace = 0;
	pd3dDescriptorRanges[4].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[5].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[5].NumDescriptors = 1;
	pd3dDescriptorRanges[5].BaseShaderRegister = 11; //t11: gtxtEmissionTexture
	pd3dDescriptorRanges[5].RegisterSpace = 0;
	pd3dDescriptorRanges[5].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[6].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[6].NumDescriptors = 1;
	pd3dDescriptorRanges[6].BaseShaderRegister = 12; //t12: gtxtEmissionTexture
	pd3dDescriptorRanges[6].RegisterSpace = 0;
	pd3dDescriptorRanges[6].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[7].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[7].NumDescriptors = 1;
	pd3dDescriptorRanges[7].BaseShaderRegister = 13; //t13: gtxtSkyBoxTexture
	pd3dDescriptorRanges[7].RegisterSpace = 0;
	pd3dDescriptorRanges[7].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[8].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[8].NumDescriptors = 1;
	pd3dDescriptorRanges[8].BaseShaderRegister = 1; //t1: gtxtTerrainBaseTexture
	pd3dDescriptorRanges[8].RegisterSpace = 0;
	pd3dDescriptorRanges[8].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[9].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[9].NumDescriptors = 1;
	pd3dDescriptorRanges[9].BaseShaderRegister = 2; //t2: gtxtTerrainDetailTexture
	pd3dDescriptorRanges[9].RegisterSpace = 0;
	pd3dDescriptorRanges[9].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[10].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[10].NumDescriptors = 1;
	pd3dDescriptorRanges[10].BaseShaderRegister = 0; //t0: gtxtTexture
	pd3dDescriptorRanges[10].RegisterSpace = 0;
	pd3dDescriptorRanges[10].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[11].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[11].NumDescriptors = 1;
	pd3dDescriptorRanges[11].BaseShaderRegister = 14; //t14: gtxtUITexture
	pd3dDescriptorRanges[11].RegisterSpace = 0;
	pd3dDescriptorRanges[11].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[12].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[12].NumDescriptors = 1;
	pd3dDescriptorRanges[12].BaseShaderRegister = 15; //t14: gtxtUIScoreTexture
	pd3dDescriptorRanges[12].RegisterSpace = 0;
	pd3dDescriptorRanges[12].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER pd3dRootParameters[20];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 1; //Camera
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 33;
	pd3dRootParameters[1].Constants.ShaderRegister = 2; //GameObject
	pd3dRootParameters[1].Constants.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[2].Descriptor.ShaderRegister = 4; //Lights
	pd3dRootParameters[2].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[3].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[3].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[0]);
	pd3dRootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[4].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[4].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[1]);
	pd3dRootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[5].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[5].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[2]);
	pd3dRootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[6].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[6].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[3]);
	pd3dRootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[7].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[7].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[4]);
	pd3dRootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[8].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[8].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[5]);
	pd3dRootParameters[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[9].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[9].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[9].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[6]);
	pd3dRootParameters[9].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[10].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[10].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[10].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[7]);
	pd3dRootParameters[10].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[11].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[11].Descriptor.ShaderRegister = 7; //Skinned Bone Offsets
	pd3dRootParameters[11].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[11].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	pd3dRootParameters[12].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[12].Descriptor.ShaderRegister = 8; //Skinned Bone Transforms
	pd3dRootParameters[12].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[12].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	pd3dRootParameters[13].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[13].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[13].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[8]);
	pd3dRootParameters[13].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[14].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[14].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[14].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[9]);
	pd3dRootParameters[14].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[15].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[15].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[15].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[10]);
	pd3dRootParameters[15].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[16].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[16].Descriptor.ShaderRegister = 5; //Framework Info
	pd3dRootParameters[16].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[16].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[17].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[17].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[17].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[11]);
	pd3dRootParameters[17].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[18].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[18].Descriptor.ShaderRegister = 3; //HP Info
	pd3dRootParameters[18].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[18].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[19].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[19].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[19].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[12]);
	pd3dRootParameters[19].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_STATIC_SAMPLER_DESC pd3dSamplerDescs[3];

	pd3dSamplerDescs[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	pd3dSamplerDescs[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pd3dSamplerDescs[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pd3dSamplerDescs[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pd3dSamplerDescs[0].MipLODBias = 0;
	pd3dSamplerDescs[0].MaxAnisotropy = 1;
	pd3dSamplerDescs[0].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	pd3dSamplerDescs[0].MinLOD = 0;
	pd3dSamplerDescs[0].MaxLOD = D3D12_FLOAT32_MAX;
	pd3dSamplerDescs[0].ShaderRegister = 0;
	pd3dSamplerDescs[0].RegisterSpace = 0;
	pd3dSamplerDescs[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dSamplerDescs[1].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	pd3dSamplerDescs[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	pd3dSamplerDescs[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	pd3dSamplerDescs[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	pd3dSamplerDescs[1].MipLODBias = 0;
	pd3dSamplerDescs[1].MaxAnisotropy = 1;
	pd3dSamplerDescs[1].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	pd3dSamplerDescs[1].MinLOD = 0;
	pd3dSamplerDescs[1].MaxLOD = D3D12_FLOAT32_MAX;
	pd3dSamplerDescs[1].ShaderRegister = 1;
	pd3dSamplerDescs[1].RegisterSpace = 0;
	pd3dSamplerDescs[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dSamplerDescs[2].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	pd3dSamplerDescs[2].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pd3dSamplerDescs[2].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pd3dSamplerDescs[2].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pd3dSamplerDescs[2].MipLODBias = 0;
	pd3dSamplerDescs[2].MaxAnisotropy = 1;
	pd3dSamplerDescs[2].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	pd3dSamplerDescs[2].MinLOD = 0;
	pd3dSamplerDescs[2].MaxLOD = D3D12_FLOAT32_MAX;
	pd3dSamplerDescs[2].ShaderRegister = 2;
	pd3dSamplerDescs[2].RegisterSpace = 0;
	pd3dSamplerDescs[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = _countof(pd3dSamplerDescs);
	d3dRootSignatureDesc.pStaticSamplers = pd3dSamplerDescs;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = nullptr;
	ID3DBlob *pd3dErrorBlob = nullptr;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&pd3dGraphicsRootSignature);
	SAFE_RELEASE(pd3dSignatureBlob);
	SAFE_RELEASE(pd3dErrorBlob);

	return pd3dGraphicsRootSignature;
}

void Scene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256의 배수
	m_pd3dcbLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, nullptr, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, nullptr);

	m_pd3dcbLights->Map(0, nullptr, (void **)&m_pcbMappedLights);
}

void Scene::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	::memcpy(m_pcbMappedLights->m_pLights, m_pLights, sizeof(LIGHT) * m_nLights);
	::memcpy(&m_pcbMappedLights->m_xmf4GlobalAmbient, &m_xmf4GlobalAmbient, sizeof(XMFLOAT4));
	::memcpy(&m_pcbMappedLights->m_nLights, &m_nLights, sizeof(int));
}

void Scene::ReleaseShaderVariables()
{
	if (m_pd3dcbLights)
	{
		m_pd3dcbLights->Unmap(0, nullptr);
		m_pd3dcbLights->Release();
	}
}

void Scene::ReleaseUploadBuffers()
{
	if (m_pSkyBox) m_pSkyBox->ReleaseUploadBuffers();

	for (int i = 0; i < m_nShaders; i++) m_ppShaders[i]->ReleaseUploadBuffers();
	for (int i = 0; i < m_nGameObjects; i++) if (m_ppGameObjects[i]) m_ppGameObjects[i]->ReleaseUploadBuffers();
	for (auto& obj : hierarchicalGameObjects) obj->ReleaseUploadBuffers();
}

void Scene::CreateCbvSrvDescriptorHeaps(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews)
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = nConstantBufferViews + nShaderResourceViews; //CBVs + SRVs 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dCbvSrvDescriptorHeap);

	m_d3dCbvCPUDescriptorNextHandle = m_d3dCbvCPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dCbvGPUDescriptorNextHandle = m_d3dCbvGPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	m_d3dSrvCPUDescriptorNextHandle.ptr = m_d3dSrvCPUDescriptorStartHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
	m_d3dSrvGPUDescriptorNextHandle.ptr = m_d3dSrvGPUDescriptorStartHandle.ptr = m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
}

D3D12_GPU_DESCRIPTOR_HANDLE Scene::CreateConstantBufferViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, ID3D12Resource *pd3dConstantBuffers, UINT nStride)
{
	D3D12_GPU_DESCRIPTOR_HANDLE d3dCbvGPUDescriptorHandle = m_d3dCbvGPUDescriptorNextHandle;
	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = pd3dConstantBuffers->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC d3dCBVDesc;
	d3dCBVDesc.SizeInBytes = nStride;
	for (int j = 0; j < nConstantBufferViews; j++)
	{
		d3dCBVDesc.BufferLocation = d3dGpuVirtualAddress + (nStride * j);
		m_d3dCbvCPUDescriptorNextHandle.ptr = m_d3dCbvCPUDescriptorNextHandle.ptr + ::gnCbvSrvDescriptorIncrementSize;
		pd3dDevice->CreateConstantBufferView(&d3dCBVDesc, m_d3dCbvCPUDescriptorNextHandle);
		m_d3dCbvGPUDescriptorNextHandle.ptr = m_d3dCbvGPUDescriptorNextHandle.ptr + ::gnCbvSrvDescriptorIncrementSize;
	}
	return(d3dCbvGPUDescriptorHandle);
}

D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(D3D12_RESOURCE_DESC d3dResourceDesc, UINT nTextureType)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc;
	d3dShaderResourceViewDesc.Format = d3dResourceDesc.Format;
	d3dShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	switch (nTextureType)
	{
		case RESOURCE_TEXTURE2D: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 1)
		case RESOURCE_TEXTURE2D_ARRAY:
			d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			d3dShaderResourceViewDesc.Texture2D.MipLevels = -1;
			d3dShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			d3dShaderResourceViewDesc.Texture2D.PlaneSlice = 0;
			d3dShaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.0f;
			break;
		case RESOURCE_TEXTURE2DARRAY: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize != 1)
			d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
			d3dShaderResourceViewDesc.Texture2DArray.MipLevels = -1;
			d3dShaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
			d3dShaderResourceViewDesc.Texture2DArray.PlaneSlice = 0;
			d3dShaderResourceViewDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
			d3dShaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
			d3dShaderResourceViewDesc.Texture2DArray.ArraySize = d3dResourceDesc.DepthOrArraySize;
			break;
		case RESOURCE_TEXTURE_CUBE: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 6)
			d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
			d3dShaderResourceViewDesc.TextureCube.MipLevels = -1;
			d3dShaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
			d3dShaderResourceViewDesc.TextureCube.ResourceMinLODClamp = 0.0f;
			break;
		case RESOURCE_BUFFER: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
			d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			d3dShaderResourceViewDesc.Buffer.FirstElement = 0;
			d3dShaderResourceViewDesc.Buffer.NumElements = 0;
			d3dShaderResourceViewDesc.Buffer.StructureByteStride = 0;
			d3dShaderResourceViewDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
			break;
	}
	return(d3dShaderResourceViewDesc);
}

D3D12_GPU_DESCRIPTOR_HANDLE Scene::CreateShaderResourceViews(ID3D12Device *pd3dDevice, Texture *pTexture, UINT nRootParameter, bool bAutoIncrement)
{
	D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGPUDescriptorHandle = m_d3dSrvGPUDescriptorNextHandle;
	if (pTexture)
	{
		int nTextures = pTexture->GetTextures();
		int nTextureType = pTexture->GetTextureType();
		for (int i = 0; i < nTextures; i++)
		{
			ID3D12Resource *pShaderResource = pTexture->GetTexture(i);
			D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();
			D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = GetShaderResourceViewDesc(d3dResourceDesc, nTextureType);
			pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, m_d3dSrvCPUDescriptorNextHandle);
			m_d3dSrvCPUDescriptorNextHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;

			pTexture->SetRootArgument(i, (bAutoIncrement) ? (nRootParameter + i) : nRootParameter, m_d3dSrvGPUDescriptorNextHandle);
			m_d3dSrvGPUDescriptorNextHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;
		}
	}
	return(d3dSrvGPUDescriptorHandle);
}

bool Scene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool Scene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		case VK_RETURN:
			break;
		case VK_F4:
			break;
		case VK_F5:
			break;
		case VK_F6:
			break;
		case VK_INSERT:
			m_pLights[43].m_xmf3Position = m_pPlayer->head->GetPosition();
			m_pLights[43].m_xmf3Position.y = m_pPlayer->head->GetPosition().y + 50.f;
			m_pLights[43].m_xmf4Ambient = XMFLOAT4(0.1f, 0.0f, 0.0f, .3f);
			m_pLights[43].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.0f, 0.0f, .3f);
			m_pLights[43].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
			m_pLights[43].m_bEnable = !m_pLights[43].m_bEnable;
			break;
		case VK_DELETE:
			m_pLights[43].m_xmf3Position = m_pPlayer->head->GetPosition();
			m_pLights[43].m_xmf3Position.y = m_pPlayer->head->GetPosition().y + 50.f;
			m_pLights[43].m_xmf4Ambient = XMFLOAT4(0.f, 0.11f, 0.f, .3f);
			m_pLights[43].m_xmf4Diffuse = XMFLOAT4(0.1f, 0.1f, 0.f, .3f);
			m_pLights[43].m_xmf4Specular = XMFLOAT4(0.1f, 0.1f, 0.f, 0.0f);
			m_pLights[43].m_bEnable = !m_pLights[43].m_bEnable;
			break;
		case VK_OEM_3:
		{
			cout << m_pPlayer->GetPosition().x << " / " << m_pPlayer->GetPosition().y << " / " << m_pPlayer->GetPosition().z << endl;
			break;
		}
		case VK_NUMPAD0:
		{
			particle->PositionInit(m_pPlayer->rHand->GetPosition());
			break;
		}
		default:
			break;
		}
		break;
	default:
		break;
	}
	return(false);
}

void Scene::Scenario()
{
	if (bScenario == false)
	{
		bScenario = true;
		hierarchicalGameObjects.data()[1]->isAlive = true;
		m_pPlayer->isAlive = true;

		hierarchicalGameObjects.data()[1]->SetPosition(XMFLOAT3(15.3046f, 10.0f, -769.689f));
		hierarchicalGameObjects.data()[1]->wayPoint.SetCurWayPoints(0);

		m_pPlayer->SetPosition(XMFLOAT3(-14.245930f, 10.0f, -769.689f));
		m_pPlayer->wayPoint.SetCurWayPoints(0);
		m_pPlayer->bScenario = true;
	}
	else
	{
		bScenario = false;
		m_pPlayer->bScenario = false;
	}
}

void Scene::Hit()
{
	if (hierarchicalGameObjects.data()[OTHERPLAYER]->hp > 0)
	{
		hierarchicalGameObjects.data()[OTHERPLAYER]->hp -= m_pPlayer->attackType;
		//hitSound->Play();
		//attackSound->Play();
		hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, m_pPlayer->nowState);
	}
	else
	{
		hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_KNOCKDOWN);
		hierarchicalGameObjects.data()[OTHERPLAYER]->isAlive = false;
	}
	m_pLights[43].m_bEnable = !m_pLights[43].m_bEnable;
}

bool Scene::ProcessInput(UCHAR *pKeysBuffer)
{
	return(false);
}

float g_time = 0.f;

void Scene::AnimateObjects(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;
	g_time += fTimeElapsed;

	for (int i = 0; i < m_nGameObjects; i++) if (m_ppGameObjects[i]) m_ppGameObjects[i]->Animate(fTimeElapsed);
	for (int i = 0; i < m_nShaders; i++) if (m_ppShaders[i]) m_ppShaders[i]->AnimateObjects(fTimeElapsed);

	for (auto& boundBox : m_pPlayer->boundBoxs)
	{
		boundBox.second->Update(fTimeElapsed, m_pPlayer->bones[boundBox.first]);
	}
	for (auto& boundBox : hierarchicalGameObjects.data()[OTHERPLAYER]->boundBoxs)
	{
		boundBox.second->Update(fTimeElapsed, hierarchicalGameObjects.data()[OTHERPLAYER]->bones[boundBox.first]);
	}

	if (bScenario)
	{
		hierarchicalGameObjects[1]->UpdateWayPoints();
		if (m_pPlayer) m_pPlayer->UpdateWayPoints();
	}
	
	int swit = 0;
	// rgb 그라데이션 해야함
	for (int i = 38; i < 40; ++i)
	{
		m_pLights[i].m_xmf3Direction = XMFLOAT3(Vector3::Normalize(Vector3::Subtract(m_pPlayer->GetPosition(), m_pLights[i].m_xmf3Position)));
		

		if (m_pLights[i].m_xmf4Diffuse.x < 1.f)
		{
			m_pLights[i].m_xmf4Diffuse = XMFLOAT4(g_time, 0.f, 0.f, 1.f);
		}
		else if (m_pLights[i].m_xmf4Diffuse.y < 1.f)
		{
			m_pLights[i].m_xmf4Diffuse = XMFLOAT4(0.f, g_time, 0.f, 1.f);
		}
		else if (m_pLights[i].m_xmf4Diffuse.z < 1.f)
		{
			m_pLights[i].m_xmf4Diffuse = XMFLOAT4(0.f, 0.f, g_time, 1.f);
		}
	}

	for (int i = 40; i < 43; ++i)
	{
		if (m_pLights[i].m_xmf3Position.z - 30.f < m_pPlayer->GetPosition().z)
		{
			m_pLights[i].m_bEnable = true;
		}
		else
		{
			m_pLights[i].m_bEnable = false;
		}

		if (m_pPlayer->GetPosition().z > -300)
		{
			m_pLights[i].m_bEnable = false;
		}
	}

	//m_pPlayer->head->objectCollision->Center = m_pPlayer->head->GetPosition();
	//m_pPlayer->lHand->objectCollision->Center = m_pPlayer->lHand->GetPosition();
	//m_pPlayer->rHand->objectCollision->Center = m_pPlayer->rHand->GetPosition();
	//m_pPlayer->spine->objectCollision->Center = m_pPlayer->spine->GetPosition();
	//hierarchicalGameObjects[OTHERPLAYER]->head->objectCollision->Center = hierarchicalGameObjects[OTHERPLAYER]->head->GetPosition();
	//hierarchicalGameObjects[OTHERPLAYER]->lHand->objectCollision->Center = hierarchicalGameObjects[OTHERPLAYER]->lHand->GetPosition();
	//hierarchicalGameObjects[OTHERPLAYER]->rHand->objectCollision->Center = hierarchicalGameObjects[OTHERPLAYER]->rHand->GetPosition();
	//hierarchicalGameObjects[OTHERPLAYER]->spine->objectCollision->Center = hierarchicalGameObjects[OTHERPLAYER]->spine->GetPosition();

	particle->Update(m_pPlayer->bones["head"]->GetPosition(), fTimeElapsed);

	if (hierarchicalGameObjects.data()[OTHERPLAYER]->hp <= 0.f)
	{
		if (ui["0_OtherPlayerTotalScore"]->isActive())
		{
			ui["0_OtherPlayerTotalScore"]->SetActive(false);
			ui["1_OtherPlayerTotalScore"]->SetActive(true);
		}
		else if (ui["1_OtherPlayerTotalScore"]->isActive())
		{
			ui["1_OtherPlayerTotalScore"]->SetActive(false);
			ui["2_OtherPlayerTotalScore"]->SetActive(true);
		}
		else if (ui["2_OtherPlayerTotalScore"]->isActive())
		{
			ui["2_OtherPlayerTotalScore"]->SetActive(false);
			ui["3_OtherPlayerTotalScore"]->SetActive(true);
		}
		m_pPlayer->hp = 100.f;
		hierarchicalGameObjects.data()[OTHERPLAYER]->hp = 100.f;
	}

	if (m_pPlayer->hp <= 0.f)
	{
		if (ui["0_PlayerTotalScore"]->isActive())
		{
			ui["0_PlayerTotalScore"]->SetActive(false);
			ui["1_PlayerTotalScore"]->SetActive(true);
		}
		else if (ui["1_PlayerTotalScore"]->isActive())
		{
			ui["1_PlayerTotalScore"]->SetActive(false);
			ui["2_PlayerTotalScore"]->SetActive(true);
		}
		else if (ui["2_PlayerTotalScore"]->isActive())
		{
			ui["2_PlayerTotalScore"]->SetActive(false);
			ui["3_PlayerTotalScore"]->SetActive(true);
		}
		m_pPlayer->hp = 100.f;
		hierarchicalGameObjects.data()[OTHERPLAYER]->hp = 100.f;
	}

	//TODO : 여기 다시보기
	CollideCageSide();

	CollidePVE();
}

void Scene::Render(ID3D12GraphicsCommandList *pd3dCommandList, Camera *pCamera)
{
	if (m_pd3dGraphicsRootSignature) pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(2, d3dcbLightsGpuVirtualAddress); //Lights

	if (m_pSkyBox) m_pSkyBox->Render(pd3dCommandList, pCamera);

	for (int i = 0; i < m_nGameObjects; i++) if (m_ppGameObjects[i]) m_ppGameObjects[i]->Render(pd3dCommandList, pCamera);
	for (int i = 0; i < m_nShaders; i++) if (m_ppShaders[i]) m_ppShaders[i]->Render(pd3dCommandList, pCamera);

	if (!hierarchicalGameObjects.data()[OTHERPLAYER]->boundBoxs.empty())
	{
		for (auto& o : hierarchicalGameObjects.data()[OTHERPLAYER]->boundBoxs)
		{
			if (o.second->boundBoxRender)
			{
				o.second->Render(pd3dCommandList, pCamera);
			}
		}
	}

	if (!m_pPlayer->boundBoxs.empty())
	{
		for (auto& o : m_pPlayer->boundBoxs)
		{
			if (o.second->boundBoxRender)
			{
				o.second->Render(pd3dCommandList, pCamera);
			}
		}
	}

	if (!hierarchicalGameObjects.empty())
	{
		for (auto& object : hierarchicalGameObjects)
		{
			object->Animate(m_fElapsedTime);
			if (object->m_pSkinnedAnimationController)
			{
				object->UpdateTransform(nullptr);
			}

			object->Render(pd3dCommandList, pCamera);
		}
	}

	particle->Render(pd3dCommandList, pCamera);




	if (!ui.empty())
	{
		for (auto& i : ui)
		{
			i.second->UpdateShaderVariables(pd3dCommandList);
			i.second->Render(pd3dCommandList, pCamera);
		}
	}

	soundManager->Update();
}

void Scene::CollideCageSide()
{
	if (cageCollision.Intersects(*m_pPlayer->playerCollision))
	{
		// 케이지 안에 있을 때 처리
		//cout << "ok\n";
		for (int i = 0; i < 4; ++i)
			m_pPlayer->canMove[i] = true;
	}
	else
	{
		// 케이지 밖에 있을 때 처리
		// TODO: 모서리 처리해야함
		// 뒤 center - radius
		if (cageCollision.Center.z - cageCollision.Radius > m_pPlayer->playerCollision->Center.z - m_pPlayer->playerCollision->Extents.z)
		{
			m_pPlayer->canMove[0] = false;
		}
		// 왼쪽 center - radius
		if (cageCollision.Center.x - cageCollision.Radius > m_pPlayer->playerCollision->Center.x - m_pPlayer->playerCollision->Extents.x)
		{
			m_pPlayer->canMove[1] = false;
		}
		// 오른쪽 center + radius
		if (cageCollision.Center.x + cageCollision.Radius < m_pPlayer->playerCollision->Center.x + m_pPlayer->playerCollision->Extents.x)
		{
			m_pPlayer->canMove[2] = false;
		}
		// 앞 center + radius
		if (cageCollision.Center.z + cageCollision.Radius < m_pPlayer->playerCollision->Center.z + m_pPlayer->playerCollision->Extents.z)
		{
			m_pPlayer->canMove[3] = false;
		}
	}
}

void Scene::CollidePVE()
{
	//UINT nState{ STATE_IDLE };

	//if (hierarchicalGameObjects[OTHERPLAYER]->nowState == STATE_IDLE/* || countTimer.GetTimeElapsed() > 3000.0f*/)
	//{
	//	aniNum = uid(dre);
	//	switch (aniNum)
	//	{
	//		//case ANIMATION_MOVE_FORWARD:
	//		//	break;
	//		//case ANIMATION_MOVE_BACKWARD:
	//		//	break;
	//		//case ANIMATION_MOVE_LEFT:
	//		//	break;
	//		//case ANIMATION_MOVE_RIGHT:
	//		//	break;
	//	case ANIMATION_HOOK_L:
	//		nState = STATE_ATTACK_LEFT_HOOK;
	//		break;
	//	case ANIMATION_HOOK_R:
	//		nState = STATE_ATTACK_RIGHT_HOOK;
	//		break;
	//	case ANIMATION_JAB:
	//		nState = STATE_ATTACK_JAB;
	//		break;
	//	case ANIMATION_GUARD_LEFT_HEAD:
	//		nState = STATE_GUARD_LEFT_HEAD;
	//		break;
	//	case ANIMATION_GUARD_RIGHT_HEAD:
	//		nState = STATE_GUARD_RIGHT_HEAD;
	//		break;
	//		//case ANIMATION_GUARD_BODY:
	//		//	break;
	//	default:
	//		break;
	//	}

	//	hierarchicalGameObjects[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, aniNum);
	//	hierarchicalGameObjects[OTHERPLAYER]->nowState = nState;

	//	//countTimer.Start();
	//}
	//else
	//{
	//	//cplayer->hp = myHP.playerHp;


	//	switch (hierarchicalGameObjects[OTHERPLAYER]->nowState)
	//	{
	//	case STATE_HIT_TORSO_LEFT:
	//		hierarchicalGameObjects[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_LEFT_A);
	//		hierarchicalGameObjects[OTHERPLAYER]->nowState = STATE_IDLE;
	//		break;
	//	case STATE_HIT_TORSO_RIGHT:
	//		hierarchicalGameObjects[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_RIGHT_A);
	//		hierarchicalGameObjects[OTHERPLAYER]->nowState = STATE_IDLE;
	//		break;
	//	case STATE_HIT_TORSO_STRIGHT:
	//		hierarchicalGameObjects[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_STRIGHT_A);
	//		hierarchicalGameObjects[OTHERPLAYER]->nowState = STATE_IDLE;
	//		break;
	//	case STATE_KNOCKDOWN:
	//		hierarchicalGameObjects[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_KNOCKDOWN);
	//		hierarchicalGameObjects[OTHERPLAYER]->isAlive = false;
	//		break;
	//	}

	//	if (m_pPlayer->rHand->objectCollision->Intersects(*hierarchicalGameObjects[OTHERPLAYER]->head->objectCollision) && !m_pPlayer->rHand->isCollide)	// 오른손과 머리
	//	{
	//		m_pPlayer->rHand->isCollide = true;
	//		cout << "RIGHT HAND - HEAD COLLIDE! " << endl;
	//		//hierarchicalGameObjects[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_LEFT_A);
	//		hierarchicalGameObjects[OTHERPLAYER]->nowState = STATE_HIT_TORSO_LEFT;
	//	}
	//	else m_pPlayer->rHand->isCollide = false;

	//	if (m_pPlayer->lHand->objectCollision->Intersects(*hierarchicalGameObjects[OTHERPLAYER]->head->objectCollision) && !m_pPlayer->lHand->isCollide)	// 왼손과 머리
	//	{
	//		m_pPlayer->lHand->isCollide = true;
	//		cout << "LEFT HAND - HEAD COLLIDE! " << endl;
	//		//hierarchicalGameObjects[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_RIGHT_A);
	//		hierarchicalGameObjects[OTHERPLAYER]->nowState = STATE_HIT_TORSO_RIGHT;
	//	}
	//	else
	//		m_pPlayer->lHand->isCollide = false;

	//	if ((m_pPlayer->rHand->objectCollision->Intersects(*hierarchicalGameObjects[OTHERPLAYER]->rHand->objectCollision)) || (m_pPlayer->rHand->objectCollision->Intersects(*hierarchicalGameObjects[OTHERPLAYER]->lHand->objectCollision)) && (m_pPlayer->nowState == STATE_GUARD_RIGHT_HEAD) && !m_pPlayer->rHand->isCollide) // 오른손과 오른/왼손 - 가드
	//	{
	//		m_pPlayer->rHand->isCollide = true;
	//		cout << "RIGHT HAND - Guard " << endl;

	//	}
	//	else
	//		m_pPlayer->rHand->isCollide = false;

	//	if ((m_pPlayer->lHand->objectCollision->Intersects(*hierarchicalGameObjects[OTHERPLAYER]->rHand->objectCollision)) || (m_pPlayer->lHand->objectCollision->Intersects(*hierarchicalGameObjects[OTHERPLAYER]->lHand->objectCollision)) && (m_pPlayer->nowState == STATE_GUARD_RIGHT_HEAD) && !m_pPlayer->lHand->isCollide) // 왼손과 오른/왼손 - 가드
	//	{
	//		m_pPlayer->lHand->isCollide = true;
	//		cout << "LEFT HAND - Guard! " << endl;
	//	}
	//	else
	//		m_pPlayer->lHand->isCollide = false;


	//	if (hierarchicalGameObjects[OTHERPLAYER]->rHand->objectCollision->Intersects(*m_pPlayer->head->objectCollision) && !hierarchicalGameObjects[OTHERPLAYER]->rHand->isCollide)	// 오른손과 머리
	//	{
	//		hierarchicalGameObjects[OTHERPLAYER]->rHand->isCollide = true;
	//		//cout << "RIGHT HAND - HEAD COLLIDE! " << endl;
	//		m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_LEFT_A);
	//		m_pPlayer->nowState = STATE_IDLE;

	//	}
	//	else hierarchicalGameObjects[OTHERPLAYER]->rHand->isCollide = false;

	//	if (hierarchicalGameObjects[OTHERPLAYER]->lHand->objectCollision->Intersects(*m_pPlayer->head->objectCollision) && !hierarchicalGameObjects[OTHERPLAYER]->lHand->isCollide)	// 왼손과 머리
	//	{
	//		hierarchicalGameObjects[OTHERPLAYER]->lHand->isCollide = true;
	//		//cout << "LEFT HAND - HEAD COLLIDE! " << endl;
	//		m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_RIGHT_A);
	//		m_pPlayer->nowState = STATE_IDLE;

	//	}
	//	else
	//		hierarchicalGameObjects[OTHERPLAYER]->lHand->isCollide = false;

	//	if ((hierarchicalGameObjects[OTHERPLAYER]->rHand->objectCollision->Intersects(*m_pPlayer->rHand->objectCollision)) || (hierarchicalGameObjects[OTHERPLAYER]->rHand->objectCollision->Intersects(*m_pPlayer->lHand->objectCollision)) && (hierarchicalGameObjects[OTHERPLAYER]->nowState == STATE_GUARD_RIGHT_HEAD) && !hierarchicalGameObjects[OTHERPLAYER]->rHand->isCollide) // 오른손과 오른/왼손 - 가드
	//	{
	//		hierarchicalGameObjects[OTHERPLAYER]->rHand->isCollide = true;
	//		//cout << "RIGHT HAND - Guard " << endl;
	//	}
	//	else
	//		hierarchicalGameObjects[OTHERPLAYER]->rHand->isCollide = false;

	//	if ((hierarchicalGameObjects[OTHERPLAYER]->lHand->objectCollision->Intersects(*m_pPlayer->rHand->objectCollision)) || (hierarchicalGameObjects[OTHERPLAYER]->lHand->objectCollision->Intersects(*m_pPlayer->lHand->objectCollision)) && (hierarchicalGameObjects[OTHERPLAYER]->nowState == STATE_GUARD_RIGHT_HEAD) && !hierarchicalGameObjects[OTHERPLAYER]->lHand->isCollide) // 왼손과 오른/왼손 - 가드
	//	{
	//		hierarchicalGameObjects[OTHERPLAYER]->lHand->isCollide = true;
	//		//cout << "LEFT HAND - Guard! " << endl;
	//	}
	//	else
	//		hierarchicalGameObjects[OTHERPLAYER]->lHand->isCollide = false;


	//	if (m_pPlayer->hp <= 0.0f)
	//	{
	//		m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_KNOCKDOWN);
	//		m_pPlayer->isAlive = false;
	//	}
	//}

	static int collideCount;
	for (auto& otherPlayerBoundBox : hierarchicalGameObjects.data()[OTHERPLAYER]->boundBoxs)
	{
		for (auto& PlayerBoundBox : m_pPlayer->boundBoxs)
		{
			if (otherPlayerBoundBox.second->m_pMesh->isIntersect(PlayerBoundBox.second->m_pMesh->obb))
			{
				if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == IDLE && m_pPlayer->nowState == ATTACK)
				{
					if (otherPlayerBoundBox.first == "Head")
					{
						hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_STRIGHT_B);
						cout << otherPlayerBoundBox.first << " is collide" << collideCount++ << endl;
						particle->PositionInit(PlayerBoundBox.second->GetPosition());
						hierarchicalGameObjects.data()[OTHERPLAYER]->hp -= 10.f;
						hierarchicalGameObjects.data()[OTHERPLAYER]->nowState = HIT;
					}
					else if (otherPlayerBoundBox.first == "Spine")
					{
						hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_KNOCKDOWN);
						cout << otherPlayerBoundBox.first << " is collide" << collideCount++ << endl;
						particle->PositionInit(PlayerBoundBox.second->GetPosition());
						hierarchicalGameObjects.data()[OTHERPLAYER]->hp -= 20.f;
						hierarchicalGameObjects.data()[OTHERPLAYER]->nowState = HIT;
					}
				}
				if (m_pPlayer->nowState == IDLE && hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == ATTACK)
				{

					if (PlayerBoundBox.first == "Head")
					{
						m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_STRIGHT_B);
						cout << PlayerBoundBox.first << " is collide" << collideCount++ << endl;
						particle->PositionInit(otherPlayerBoundBox.second->GetPosition());
						m_pPlayer->nowState = HIT;
					}
					else if (PlayerBoundBox.first == "Spine")
					{
						m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_KNOCKDOWN);
						cout << PlayerBoundBox.first << " is collide" << collideCount++ << endl;
						particle->PositionInit(otherPlayerBoundBox.second->GetPosition());
						m_pPlayer->nowState = HIT;
					}
				}
			}
		}
	}

}