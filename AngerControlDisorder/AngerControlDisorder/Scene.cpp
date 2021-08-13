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
#include "PlaneObject.h"
#include "Camera.h"

//////////Server///////////
#include "Server.h"
extern Server* server;
////////////////////////////
extern Camera* gCamera;

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

UINT aniNum{ ANIMATION_HOOK_L };

GameTimer countTimer{};

bool gStart = false;

Scene::Scene()
{
	SoundManager::Init();

	soundManager = new SoundManager("Sound/ACD_mini.mid", true);
	
	hitSound = new SoundManager("Sound/Hit01.mp3");
	attackSound = new SoundManager("Sound/Punch01.mp3");

	
	
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
		m_pLights[i].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
		m_pLights[i].m_fFalloff = 100.0f;
		m_pLights[i].m_fPhi = (float)cos(XMConvertToRadians(60.0f));
		m_pLights[i].m_fTheta = (float)cos(XMConvertToRadians(20.0f));
	}

}

void Scene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);


	CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 64, 256);

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
	ModelInfo* BoxerModel = nullptr;


#ifdef _WITH_SERVER_CONNECT

	if (server->thread_id.thread_num == 1)
	{
		
		BoxerModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/OtherPlayer.bin", nullptr);
	}
	else
	{
		
		BoxerModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/TEST2.bin", nullptr);
	}
#else
	BoxerModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/OtherPlayer.bin", nullptr);
#endif

	Object* boxer = new BoxerObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, BoxerModel, 1);
	boxer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_COMBAT_MODE_A);
	for (int i = 0; i < boxer->m_pSkinnedAnimationController->m_pAnimationSets->m_nAnimationSets; ++i)
		boxer->m_pSkinnedAnimationController->m_pAnimationSets->m_pAnimationSets[i]->isOtherPlayer = true;
	
	// position
	boxer->SetPosition(-1.0f, 8.5f, 30.0f);
	boxer->Rotate(0.0f, 180.0f, 0.0f);




	hierarchicalGameObjects.push_back(boxer);
	SAFE_DELETE(BoxerModel);


	for (auto& o : hierarchicalGameObjects.data()[OTHERPLAYER]->boundBoxs)
	{
		o.second->boundBoxRender = !o.second->boundBoxRender;
	}

	ModelInfo* crowdModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/BoxingComplete.bin", nullptr);

	int nFloors = 4;
	size_t nBaseModels = hierarchicalGameObjects.size();
	int nCrowds = 9;

	float angle = 0.0f;
	float radius = 130.0f;

	for (int i = 0; i < nFloors; ++i)
	{
		angle = -30.0f;
		for (int j = (int)nBaseModels + i * nCrowds; j < (int)nBaseModels + (i + 1) * nCrowds; ++j)
		{
			Object* crowd = new CrowdObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, crowdModel, 1);
			crowd->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
			crowd->SetPosition(cos(XMConvertToRadians(angle)) * radius, -9.0f + 4.0f * i, sin(XMConvertToRadians(angle)) * radius);
			crowd->Rotate(0.0f, 180.0f, 0.0f);
			hierarchicalGameObjects.push_back(crowd);
			angle += 30.f;
		}
		radius += 25.0f;
	}
	
	SAFE_DELETE(crowdModel);



	gGameObject = hierarchicalGameObjects;
	ui["1_BloodEffect"] = new UI_BloodEffect(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/bloodUI.dds");
	ui["1_BloodEffect"]->SetActive(false);
	
	ui["2_PlayerHP"] = new UI_HP_Player(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/DDSfile/HPBar_Other.dds");
	ui["2_OtherPlayerHP"] = new UI_HP_OtherPlayer(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/DDSfile/HPBar.dds");
	ui["2_PlayerHPBack"] = new UI_HPBackGround_Player(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/hpBack.dds");
	ui["2_OtherPlayerHPBack"] = new UI_HPBackGround_OtherPlayer(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/hpBack.dds");
	ui["2_OtherPlayerHPBack"]->SetActive(false);
	ui["2_PlayerHPBack"]->SetActive(false);
	ui["2_PlayerHP"]->SetActive(false);
	ui["2_OtherPlayerHP"]->SetActive(false);

	ui["3_PlayerTotalScore"] = new UI_PlayerTotalScore(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/DDSfile/Points_Full.dds");
	ui["3_OtherPlayerTotalScore"] = new UI_OtherPlayerTotalScore(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/DDSfile/Points_Full.dds");
	ui["3_PlayerTotalScore"]->SetActive(false);
	ui["3_OtherPlayerTotalScore"]->SetActive(false);

	ui["timerBar"] = new UI_TimerBar(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/Timer.dds");
	ui["timerBar"]->SetActive(false);

	ui["title"] = new UI_BloodEffect(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/title.dds");
	ui["title"]->SetActive(true);

	ui["ready"] = new UI_ReadyFight(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/ready.png");
	ui["fight"] = new UI_ReadyFight(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/fight.png");
	ui["ready"]->SetActive(false);
	ui["fight"]->SetActive(false);
	
	ui["youWin"] = new UI_LOSEWIN(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/youwin.dds");
	ui["youLose"] = new UI_LOSEWIN(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"UI/youlose.dds");
	ui["youWin"]->SetActive(false);
	ui["youLose"]->SetActive(false);
	particle = new Particle;
	particle->Init(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);

	effectManager = new EffectManager(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);

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
	pd3dDescriptorRanges[10].BaseShaderRegister = 0; //t0: gtxtTexture,space0
	pd3dDescriptorRanges[10].RegisterSpace = 0;
	pd3dDescriptorRanges[10].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[11].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[11].NumDescriptors = 1;
	pd3dDescriptorRanges[11].BaseShaderRegister = 14; //t14: gtxtUITexture
	pd3dDescriptorRanges[11].RegisterSpace = 0;
	pd3dDescriptorRanges[11].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[12].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[12].NumDescriptors = 1;
	pd3dDescriptorRanges[12].BaseShaderRegister = 15; //t15: gtxtUIScoreTexture
	pd3dDescriptorRanges[12].RegisterSpace = 0;
	pd3dDescriptorRanges[12].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER pd3dRootParameters[22];

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

	pd3dRootParameters[20].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[20].Descriptor.ShaderRegister = 6; //HP Info
	pd3dRootParameters[20].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[20].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[21].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[21].Descriptor.ShaderRegister = 9; //Timer Info
	pd3dRootParameters[21].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[21].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

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
		case 'Y':
		{
			m_pPlayer->hp += 0.04f;
			break;
		}
		case 'U':
		{
			m_pPlayer->PrintWorld();
			hierarchicalGameObjects.data()[OTHERPLAYER]->PrintWorld();
			break;
		}
		case 'T':
		{
#ifdef _WITH_SERVER_CONNECT
			server->SendPlayerMove.Ready = true;
			cout << "시작입력 " << endl;
#endif // _WITH_SERVER_CONNECT
			break;
		}
		case 'I':
		{
			ui["title"]->SetActive(false);
			ui["2_PlayerHP"]->SetActive(true);
			ui["2_OtherPlayerHP"]->SetActive(true);
			ui["3_PlayerTotalScore"]->SetActive(true);
			ui["3_OtherPlayerTotalScore"]->SetActive(true);
			break;
		}
		case 'O':
		{
			break;
		}

		case 'Q':
		case 'q': // 상단 주먹
		{
			if (rand() % 2)
			{
				m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HOOK_L, ANIMATION_TYPE_ONCE, true);
#ifdef _WITH_SERVER_CONNECT
				server->send_attackAnddefend.ani_num = ANIMATION_HOOK_L;
				server->send_attackAnddefend.checkAni = true;
#endif // _WITH_SERVER_CONNECT
			}
			else
			{
				m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HOOK_R, ANIMATION_TYPE_ONCE, true);
#ifdef _WITH_SERVER_CONNECT
				server->send_attackAnddefend.ani_num = ANIMATION_HOOK_R;
				server->send_attackAnddefend.checkAni = true;
#endif // _WITH_SERVER_CONNECT
			}
			break;
		}
		case 'W':
		case 'w': // 상단 킥
		{
			m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_AXE_KICK_R, ANIMATION_TYPE_ONCE, true);
#ifdef _WITH_SERVER_CONNECT
			server->send_attackAnddefend.ani_num = ANIMATION_AXE_KICK_R;
			server->send_attackAnddefend.checkAni = true;
#endif // _WITH_SERVER_CONNECT
			break;
		}
		case 'E': // 상단 가드
		case 'e':
		{
			// ANIMATION_GUARD_HOOK_LEFT
			// ANIMATION_GUARD_HOOK_RIGHT
			if (rand() % 2)
			{
				m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_GUARD_HOOK_LEFT, ANIMATION_TYPE_ONCE, true);
#ifdef _WITH_SERVER_CONNECT
				server->send_attackAnddefend.ani_num = ANIMATION_GUARD_HOOK_LEFT;
				server->send_attackAnddefend.checkAni = true;
#endif // _WITH_SERVER_CONNECT
			}
			else
			{
				m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_GUARD_HOOK_RIGHT, ANIMATION_TYPE_ONCE, true);
#ifdef _WITH_SERVER_CONNECT
				server->send_attackAnddefend.ani_num = ANIMATION_GUARD_HOOK_RIGHT;
				server->send_attackAnddefend.checkAni = true;
#endif // _WITH_SERVER_CONNECT
			}
			m_pPlayer->nowState = HIGH_GUARD;
			break;
		}
		case 'A':
		case 'a': // 중단 킥
		{
			if (rand() % 2)
			{
				m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_KICK_MID_L, ANIMATION_TYPE_ONCE, true);
#ifdef _WITH_SERVER_CONNECT
				server->send_attackAnddefend.ani_num = ANIMATION_KICK_MID_L;
				server->send_attackAnddefend.checkAni = true;
#endif // _WITH_SERVER_CONNECT
			}
			else
			{
				m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_SIDE_KICK_L, ANIMATION_TYPE_ONCE, true);
#ifdef _WITH_SERVER_CONNECT
				server->send_attackAnddefend.ani_num = ANIMATION_SIDE_KICK_L;
				server->send_attackAnddefend.checkAni = true;
#endif // _WITH_SERVER_CONNECT
			}
			
			break;
		}
		case 'D':
		case 'd': // 중단 가드
		{
			// ANIMATION_GUARD_SIDE_LEFT
			// ANIMATION_GUARD_SIDE_RIGHT
			if (rand() % 2)
			{
				m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_GUARD_SIDE_LEFT, ANIMATION_TYPE_ONCE, true);
#ifdef _WITH_SERVER_CONNECT
				server->send_attackAnddefend.ani_num = ANIMATION_GUARD_SIDE_LEFT;
				server->send_attackAnddefend.checkAni = true;
#endif // _WITH_SERVER_CONNECT
			}
			else
			{
				m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_GUARD_SIDE_RIGHT, ANIMATION_TYPE_ONCE, true);
#ifdef _WITH_SERVER_CONNECT
				server->send_attackAnddefend.ani_num = ANIMATION_GUARD_SIDE_RIGHT;
				server->send_attackAnddefend.checkAni = true;
#endif // _WITH_SERVER_CONNECT
			}
			m_pPlayer->nowState = MIDDLE_GUARD;
			break;
		}
		case 'Z':
		case 'z': // 중단 주먹
		{
			if (rand() % 2)
			{
				m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_LEFT_BODY_HOOK, ANIMATION_TYPE_ONCE, true);
#ifdef _WITH_SERVER_CONNECT
				server->send_attackAnddefend.ani_num = ANIMATION_LEFT_BODY_HOOK;
				server->send_attackAnddefend.checkAni = true;
#endif // _WITH_SERVER_CONNECT
			}
			else
			{
				m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_RIGHT_BODY_HOOK, ANIMATION_TYPE_ONCE, true);
#ifdef _WITH_SERVER_CONNECT
				server->send_attackAnddefend.ani_num = ANIMATION_RIGHT_BODY_HOOK;
				server->send_attackAnddefend.checkAni = true;
#endif // _WITH_SERVER_CONNECT
			}
			break;
		}
		case 'X':
		case 'x': // 하단 킥
		{
			if (rand() % 2)
			{
				m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_LOW_KICK_R, ANIMATION_TYPE_ONCE, true);
#ifdef _WITH_SERVER_CONNECT
				server->send_attackAnddefend.ani_num = ANIMATION_LOW_KICK_R;
				server->send_attackAnddefend.checkAni = true;
#endif // _WITH_SERVER_CONNECT
			}
			else
			{
				m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_LOW_KICK_SL, ANIMATION_TYPE_ONCE, true);
#ifdef _WITH_SERVER_CONNECT
				server->send_attackAnddefend.ani_num = ANIMATION_LOW_KICK_SL;
				server->send_attackAnddefend.checkAni = true;
#endif // _WITH_SERVER_CONNECT
			}
			break;
		}
		case 'C':
		case 'c': // 하단 가드
		{
			// ANIMATION_GUARD_LOW_LEFT
			// ANIMATION_GUARD_LOW_RIGHT
			if (rand() % 2)
			{
				m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_GUARD_LOW_LEFT, ANIMATION_TYPE_ONCE, true);
#ifdef _WITH_SERVER_CONNECT
				server->send_attackAnddefend.ani_num = ANIMATION_GUARD_LOW_LEFT;
				server->send_attackAnddefend.checkAni = true;
#endif // _WITH_SERVER_CONNECT
			}
			else
			{
				m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_GUARD_LOW_RIGHT, ANIMATION_TYPE_ONCE, true);
#ifdef _WITH_SERVER_CONNECT
				server->send_attackAnddefend.ani_num = ANIMATION_GUARD_LOW_RIGHT;
				server->send_attackAnddefend.checkAni = true;
#endif // _WITH_SERVER_CONNECT
			}
			m_pPlayer->nowState = LOW_GUARD;
			break;
		}
		case '6':
		{
			gStart = true;
			break;
		}
		}
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
	static float gameStartDelay = 0.f;
	static bool gameStart = false;


#ifdef _WITH_SERVER_CONNECT
	if (server->RecvOtherPlayerMove.Start)
	{
		gameStart = server->RecvOtherPlayerMove.Start;
	}
#else 

#endif
	if (gameStart)
	{
		gameStartDelay += fTimeElapsed;

		if (gameStartDelay >= 2.f)
		{
			soundManager->Play();
			GameTimeElapsed = 0.f;
			ui["ready"]->SetActive(true);
			ui["title"]->SetActive(false);
			ui["2_PlayerHP"]->SetActive(true);
			ui["2_OtherPlayerHPBack"]->SetActive(true);
			ui["2_PlayerHPBack"]->SetActive(true);
			ui["2_OtherPlayerHP"]->SetActive(true);
			ui["3_PlayerTotalScore"]->SetActive(true);
			ui["3_OtherPlayerTotalScore"]->SetActive(true);
			ui["timerBar"]->SetActive(true);
			m_pPlayer->Rotate(0.f, 90.f, 0.f);
			gameStart = false;
		}
	}


	m_fElapsedTime = fTimeElapsed;
	g_time += fTimeElapsed;
	GameTimeElapsed += fTimeElapsed;

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

	if (particle)
	{
		particle->Update(m_pPlayer->bones["Head"]->GetPosition(), fTimeElapsed);
	}
	if (effectManager)
	{
		effectManager->Update(fTimeElapsed, XMFLOAT3());
	}

	// UI : 왼쪽이 플레이어, 오른쪽이 다른플레이어

	if (m_pPlayer->score == 0 || hierarchicalGameObjects.data()[OTHERPLAYER]->score == 3)
	{
		for (auto& i : ui)
		{
			i.second->SetActive(false);
		}

		if (m_pPlayer->score == 0)
		{
			winPlayer = m_pPlayer;
			losePlayer = hierarchicalGameObjects.data()[OTHERPLAYER];
			ui["youWin"]->SetActive(true);
			
		}
		else if(hierarchicalGameObjects.data()[OTHERPLAYER]->score == 3)
		{
			losePlayer = m_pPlayer;
			winPlayer = hierarchicalGameObjects.data()[OTHERPLAYER];
			ui["youLose"]->SetActive(true);
		}
		

		EndGame = true;
	}

	if (EndGame == false)
	{
		// 체력이 다 되었을 때
		if (hierarchicalGameObjects.data()[OTHERPLAYER]->hp >= 0.8f)
		{
			ui["ready"]->SetActive(true);
			m_pPlayer->score -= 1;

			m_pPlayer->hp = 0.f;
			hierarchicalGameObjects.data()[OTHERPLAYER]->hp = 0.f;
		}
		else if (m_pPlayer->hp >= 0.8f)
		{
			ui["ready"]->SetActive(true);
			hierarchicalGameObjects.data()[OTHERPLAYER]->score += 1;

			m_pPlayer->hp = 0.f;
			hierarchicalGameObjects.data()[OTHERPLAYER]->hp = 0.f;
		}


		// 타임 아웃일때
		if (GameTimeElapsed >= 60.f)
		{
			// 체력이 더 많은 사람의 승리 조건
			if (m_pPlayer->hp > hierarchicalGameObjects.data()[OTHERPLAYER]->hp)
			{
				hierarchicalGameObjects.data()[OTHERPLAYER]->score += 1;
			}
			else if (m_pPlayer->hp - hierarchicalGameObjects.data()[OTHERPLAYER]->hp == EPSILON)
			{
				hierarchicalGameObjects.data()[OTHERPLAYER]->score += 1;
				m_pPlayer->score -= 1;
			}
			else if (m_pPlayer->hp < hierarchicalGameObjects.data()[OTHERPLAYER]->hp)
			{
				m_pPlayer->score -= 1;
			}
			m_pPlayer->hp = 0.f;
			hierarchicalGameObjects.data()[OTHERPLAYER]->hp = 0.f;
			ui["ready"]->SetActive(true);
		}


		static float readyTime = 0.f;
		static float fightTime = 0.f;

		static bool rotate = true;

		if (ui["ready"]->isActive())
		{
#ifdef _WITH_SERVER_CONNECT
			if (server->thread_id.thread_num == 1)
			{
				m_pPlayer->SetPosition(XMFLOAT3(-1.0f, 8.5f, -30.0f));
			}
			else
			{
				m_pPlayer->SetPosition(XMFLOAT3(-1.0f, 8.5f, 30.0f));
				if (rotate == true)
				{
					m_pPlayer->Rotate(0.0f, 180.0f, 0.0f);
					rotate = false;
				}
			}
#else
			m_pPlayer->SetPosition(XMFLOAT3(-1.0f, 8.5f, -30.0f));
#endif
			GameTimeElapsed = 0.f;
			readyTime += fTimeElapsed;
			if (readyTime >= 1.5f)
			{
				ui["ready"]->SetActive(false);
				ui["fight"]->SetActive(true);
				readyTime = 0.f;
			}
		}

		if (ui["fight"]->isActive())
		{
			fightTime += fTimeElapsed;
			if (fightTime >= 1.f)
			{
				ui["fight"]->SetActive(false);
				fightTime = 0.f;
			}
		}

	}
	else
	{
		// 게임이 끝났을 때의 연출

#ifdef _WITH_SERVER_CONNECT

		if (server->thread_id.thread_num == 1)
		{
			if (m_pPlayer == winPlayer)
			{
				m_pPlayer->SetRight(-1.f, 0.f, 0.f);
				m_pPlayer->SetUp(0.f, 1.f, 0.f);
				m_pPlayer->SetLook(0.f, 0.f, -1.f);
				m_pPlayer->SetPosition({ -5.f, 8.5f, 0.f });
			}
			else
			{
				m_pPlayer->SetRight(-1.f, 0.f, 0.f);
				m_pPlayer->SetUp(0.f, 1.f, 0.f);
				m_pPlayer->SetLook(0.f, 0.f, -1.f);
				m_pPlayer->SetPosition({ -5.f, 8.5f, 0.f });
			}
		}
		else
		{
			if (m_pPlayer == losePlayer)
			{

				m_pPlayer->SetRight(-1.f, 0.f, 0.f);
				m_pPlayer->SetUp(0.f, 1.f, 0.f);
				m_pPlayer->SetLook(0.f, 0.f, -1.f);
				m_pPlayer->SetPosition({ 5.f, 8.5f, 0.f });
			}
			else
			{
				m_pPlayer->SetRight(-1.f, 0.f, 0.f);
				m_pPlayer->SetUp(0.f, 1.f, 0.f);
				m_pPlayer->SetLook(0.f, 0.f, -1.f);
				m_pPlayer->SetPosition({ 5.f, 8.5f, 0.f });
			}
		}
#else
		winPlayer->SetRight(-1.f, 0.f, 0.f);
		winPlayer->SetUp(0.f, 1.f, 0.f);
		winPlayer->SetLook(0.f, 0.f, -1.f);
		winPlayer->SetPosition(5.f, 8.5f, 0.f);

		losePlayer->SetRight(-1.f, 0.f, 0.f);
		losePlayer->SetUp(0.f, 1.f, 0.f);
		losePlayer->SetLook(0.f, 0.f, -1.f);
		losePlayer->SetPosition(-5.f, 8.5f, 0.f);
#endif


		m_pPlayer->GetCamera()->SetLook({ -0.0223675f, -0.275638f, 0.961001f });
		m_pPlayer->GetCamera()->SetRight({ 0.999729f, -4.51691e-08f, 0.0232689f });
		m_pPlayer->GetCamera()->SetUp({ -0.00641375f, 0.961262f, 0.275563f });
		m_pPlayer->GetCamera()->SetPosition({ 0.367214f, 27.3884f, -26.9444f });

		// TODO : 카메라 월드 좌표를 여기서 Set하기
		// TODO : 플레이어 카메라로 SetLookAt 해주거나 월드좌표 받아서 Set하기

		//hierarchicalGameObjects.data()[OTHERPLAYER]->Rotate(0.f, 180.f, 0.f);
		// 세레모니
		winPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_WINN_BATTLE, ANIMATION_TYPE_LOOP);
		losePlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_LOSTING_BATTLE, ANIMATION_TYPE_LOOP);
	}

	if (server && server->positionRecv)
	{
		XMVECTOR target = { server->other_player.player_world._41 , server->other_player.player_world._42, server->other_player.player_world._43 };
		XMVECTOR nowposition = { server->oldPlayerPosition._41 , server->oldPlayerPosition._42, server->oldPlayerPosition._43 };

		nowposition = XMVectorLerp(nowposition, target, 0.5);

		XMFLOAT3 position;

		XMStoreFloat3(&position, nowposition);
		hierarchicalGameObjects[1]->SetPosition(position);
		hierarchicalGameObjects[1]->SetRight(server->player_right.x, server->player_right.y, server->player_right.z);
		hierarchicalGameObjects[1]->SetUp(server->player_up.x, server->player_up.y, server->player_up.z);
		hierarchicalGameObjects[1]->SetLook(server->player_look.x, server->player_look.y, server->player_look.z);
	}
	CollideCageSide();

	CollidePVE(fTimeElapsed);
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

	if (particle)
	{
		particle->Render(pd3dCommandList, pCamera);
	}

	if (!ui.empty())
	{
		for (auto& i : ui)
		{
			i.second->UpdateShaderVariables(pd3dCommandList);
			i.second->Render(pd3dCommandList, pCamera);
		}
	}

	if (effectManager)
	{
		effectManager->Render(pd3dCommandList, pCamera);
	}
	attackSound->Update();
	soundManager->Update();
}

void Scene::CollideCageSide()
{
	if (cageCollision.Intersects(*m_pPlayer->playerCollision))
	{
		for (int i = 0; i < 4; ++i)
			m_pPlayer->canMove[i] = true;
	}
	else
	{
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

void Scene::CollidePVE(const float& deltaTime)
{
	static float CoolTime = 0.f;
	static bool CoolDown = true;
	CoolTime += deltaTime;

	if (CoolTime >= 0.3f)
	{
		CoolDown = true;
	}

	static float effectTime = 3.f;

	effectTime += deltaTime;

	if (effectTime >= 1.f)
	{
		ui["1_BloodEffect"]->SetActive(false);
	}
	else
	{
		ui["1_BloodEffect"]->SetActive(true);
	}

	UINT playerAnimation = m_pPlayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0);
	UINT otherPlayerAnimation = hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0);

	for (auto& otherPlayerBoundBox : hierarchicalGameObjects.data()[OTHERPLAYER]->boundBoxs)
	{
		for (auto& PlayerBoundBox : m_pPlayer->boundBoxs)
		{
			if (otherPlayerBoundBox.second->m_pMesh->isIntersect(PlayerBoundBox.second->m_pMesh->obb) && CoolDown)
			{
				/*********************************************************상대 플레이어에 대한 충돌처리*****************************************************************/
				if (otherPlayerBoundBox.first == "Head" &&
					(PlayerBoundBox.first == "lHand" || PlayerBoundBox.first == "rHand" ||
						PlayerBoundBox.first == "rFoot" || PlayerBoundBox.first == "lFoot"))
				{
					if (PlayerBoundBox.first == "lHand" || PlayerBoundBox.first == "rHand")
					{
						if (playerAnimation == ANIMATION_HOOK_L || playerAnimation == ANIMATION_HOOK_R || playerAnimation == ANIMATION_ONE_TWO ||
							playerAnimation == ANIMATION_1_2_KICK || playerAnimation == ANIMATION_UPPER_CUT_L || playerAnimation == ANIMATION_1_2_3_KICK)
						{
							if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == HIGH_GUARD && m_pPlayer->nowState == ATTACK)
							{
								hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_COME_HERE_BRUCE_LI);
							}
							else if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == OTHER && m_pPlayer->nowState == ATTACK)
							{
								continue;
							}
							// 상단 가드 실패
							else
							{
								hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_HEAD_STRIGHT_B);
								if (effectManager)
								{
									effectManager->EffectOn(PlayerBoundBox.second->GetPosition(), 2);
								}
								hierarchicalGameObjects.data()[OTHERPLAYER]->hp += 0.05f;
								CoolTime = 0.f;
								CoolDown = false;
								gCamera->ZoomIn();
								attackSound->Play();
								if (particle)
								{
									particle->PositionInit(PlayerBoundBox.second->GetPosition(), PlayerBoundBox.second->GetLook(), BLUECOLOR);
								}
							}
						}
					}
					else if (PlayerBoundBox.first == "rFoot" || PlayerBoundBox.first == "lFoot")
					{
						if (playerAnimation == ANIMATION_AXE_KICK_R || playerAnimation == ANIMATION_KICK_COMBO_HEAD || playerAnimation == ANIMATION_ONE_TWO ||
							playerAnimation == ANIMATION_1_2_3_KICK)
						{
							if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == HIGH_GUARD && m_pPlayer->nowState == ATTACK)
							{
								hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_COME_HERE_BRUCE_LI);
							}
							else if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == OTHER && m_pPlayer->nowState == ATTACK)
							{
								continue;
							}
							// 상단 가드 실패
							else
							{
								hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_HEAD_STRIGHT_B);
								if (effectManager)
								{
									effectManager->EffectOn(PlayerBoundBox.second->GetPosition(), 2);
								}
								hierarchicalGameObjects.data()[OTHERPLAYER]->hp += 0.08f;
								CoolTime = 0.f;
								CoolDown = false;
								gCamera->ZoomIn();
								attackSound->Play();
								if (particle)
								{
									particle->PositionInit(PlayerBoundBox.second->GetPosition(), PlayerBoundBox.second->GetLook(), BLUECOLOR);
								}
							}
						}
					}
				}
				else if (otherPlayerBoundBox.first == "Spine" &&
					(PlayerBoundBox.first == "lHand" || PlayerBoundBox.first == "rHand" ||
						PlayerBoundBox.first == "rFoot" || PlayerBoundBox.first == "lFoot"))
				{
					if (PlayerBoundBox.first == "lHand" || PlayerBoundBox.first == "rHand")
					{
						if (playerAnimation == ANIMATION_LEFT_BODY_HOOK || playerAnimation == ANIMATION_RIGHT_BODY_HOOK)
						{
							if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == MIDDLE_GUARD && m_pPlayer->nowState == ATTACK)
							{
								hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_COME_HERE_1HAND);
							}
							else if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == OTHER && m_pPlayer->nowState == ATTACK)
							{
								continue;
							}
							else
							{
								hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_STRIGHT_B);
								if (effectManager)
								{
									effectManager->EffectOn(PlayerBoundBox.second->GetPosition(), 2);
								}
								hierarchicalGameObjects.data()[OTHERPLAYER]->hp += 0.05f;
								CoolTime = 0.f;
								CoolDown = false;
								gCamera->ZoomIn();
								attackSound->Play();
								if (particle)
								{
									particle->PositionInit(PlayerBoundBox.second->GetPosition(), PlayerBoundBox.second->GetLook(), REDCOLOR);
								}
							}
						}
					}
					else if (PlayerBoundBox.first == "rFoot" || PlayerBoundBox.first == "lFoot")
					{
						if (playerAnimation == ANIMATION_SIDE_KICK_L || playerAnimation == ANIMATION_1_2_3_KICK || playerAnimation == ANIMATION_KICK_MID_L)
						{
							if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == MIDDLE_GUARD && m_pPlayer->nowState == ATTACK)
							{
								hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_COME_HERE_1HAND);
							}
							else if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == OTHER && m_pPlayer->nowState == ATTACK)
							{
								continue;
							}
							else
							{
								hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_STRIGHT_B);
								if (effectManager)
								{
									effectManager->EffectOn(PlayerBoundBox.second->GetPosition(), 2);
								}
								hierarchicalGameObjects.data()[OTHERPLAYER]->hp += 0.08f;
								CoolTime = 0.f;
								CoolDown = false;
								gCamera->ZoomIn();
								attackSound->Play();
								if (particle)
								{
									particle->PositionInit(PlayerBoundBox.second->GetPosition(), PlayerBoundBox.second->GetLook(), REDCOLOR);
								}
							}
						}
					}
				}
				else if ((otherPlayerBoundBox.first == "lCalf" || otherPlayerBoundBox.first == "rCalf") &&
					(PlayerBoundBox.first == "rFoot" || PlayerBoundBox.first == "lFoot"))
				{
					// 하단 공격은 다리만 맞게 해야한다.
					if (playerAnimation == ANIMATION_LOW_KICK_R || playerAnimation == ANIMATION_LOW_KICK_SL || playerAnimation == ANIMATION_1_2_KICK 
						|| playerAnimation == ANIMATION_KICK_COMBO_HEAD)
					{
						if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == LOW_GUARD && m_pPlayer->nowState == ATTACK)
						{
							hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_COME_HERE_1HAND);
						}
						else if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == OTHER && m_pPlayer->nowState == ATTACK)
						{
							continue;
						}
						else
						{
							hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_STRIGHT_B);
							if (effectManager)
							{
								effectManager->EffectOn(PlayerBoundBox.second->GetPosition(), REDX);
							}
							hierarchicalGameObjects.data()[OTHERPLAYER]->hp += 0.05f;
							CoolTime = 0.f;
							CoolDown = false;
							gCamera->ZoomIn();
							attackSound->Play();
							if (particle)
							{
								particle->PositionInit(PlayerBoundBox.second->GetPosition(), PlayerBoundBox.second->GetLook(), GREENCOLOR);
							}
						}
					}
				}
				/*********************************************************본인 플레이어에 대한 충돌처리*****************************************************************/

				if (PlayerBoundBox.first == "Head" &&
					(otherPlayerBoundBox.first == "lHand" || otherPlayerBoundBox.first == "rHand" ||
						otherPlayerBoundBox.first == "rFoot" || otherPlayerBoundBox.first == "lFoot"))
				{
					if (otherPlayerBoundBox.first == "lHand" || otherPlayerBoundBox.first == "rHand")
					{
						if (otherPlayerAnimation == ANIMATION_HOOK_L || otherPlayerAnimation == ANIMATION_HOOK_R || otherPlayerAnimation == ANIMATION_ONE_TWO ||
							otherPlayerAnimation == ANIMATION_1_2_KICK || otherPlayerAnimation == ANIMATION_UPPER_CUT_L || otherPlayerAnimation == ANIMATION_1_2_3_KICK)
						{
							if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == ATTACK && m_pPlayer->nowState == HIGH_GUARD)
							{
								m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_COME_HERE_BRUCE_LI);
							}
							else if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == ATTACK && m_pPlayer->nowState == OTHER)
							{
								continue;
							}
							// 상단 가드 실패
							else
							{
								m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_HEAD_STRIGHT_B);
								if (effectManager)
								{
									effectManager->EffectOn(otherPlayerBoundBox.second->GetPosition(), 2);
								}
								m_pPlayer->hp += 0.05f;
								CoolTime = 0.f;
								CoolDown = false;
								effectTime = 0.f;
								if (particle)
								{
									particle->PositionInit(PlayerBoundBox.second->GetPosition(), otherPlayerBoundBox.second->GetLook(), BLUECOLOR);
								}
							}
						}
					}
					else if (otherPlayerBoundBox.first == "rFoot" || otherPlayerBoundBox.first == "lFoot")
					{
						if (otherPlayerAnimation == ANIMATION_AXE_KICK_R || otherPlayerAnimation == ANIMATION_KICK_COMBO_HEAD || otherPlayerAnimation == ANIMATION_ONE_TWO ||
							otherPlayerAnimation == ANIMATION_KICK_COMBO_HEAD)
						{
							if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == ATTACK && m_pPlayer->nowState == HIGH_GUARD)
							{
								m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_COME_HERE_BRUCE_LI);
							}
							else if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == ATTACK && m_pPlayer->nowState == OTHER)
							{
								continue;
							}
							// 상단 가드 실패
							else
							{
								m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_HEAD_STRIGHT_B);
								if (effectManager)
								{
									effectManager->EffectOn(otherPlayerBoundBox.second->GetPosition(), 2);
								}
								m_pPlayer->hp += 0.08f;
								CoolTime = 0.f;
								CoolDown = false;
								effectTime = 0.f;
								if (particle)
								{
									particle->PositionInit(PlayerBoundBox.second->GetPosition(), otherPlayerBoundBox.second->GetLook(), BLUECOLOR);
								}
							}
						}
					}
				}
				else if (PlayerBoundBox.first == "Spine" &&
					(otherPlayerBoundBox.first == "lHand" || otherPlayerBoundBox.first == "rHand" ||
						otherPlayerBoundBox.first == "rFoot" || otherPlayerBoundBox.first == "lFoot"))
				{
					if (otherPlayerBoundBox.first == "lHand" || otherPlayerBoundBox.first == "rHand")
					{
						if (otherPlayerAnimation == ANIMATION_LEFT_BODY_HOOK || otherPlayerAnimation == ANIMATION_RIGHT_BODY_HOOK)
						{
							if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == ATTACK && m_pPlayer->nowState == MIDDLE_GUARD)
							{
								m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_COME_HERE_1HAND);
							}
							else if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == ATTACK && m_pPlayer->nowState == OTHER)
							{
								continue;
							}
							else
							{
								m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_STRIGHT_B);
								if (effectManager)
								{
									effectManager->EffectOn(otherPlayerBoundBox.second->GetPosition(), 2);
								}
								m_pPlayer->hp += 0.05f;
								CoolTime = 0.f;
								CoolDown = false;
								effectTime = 0.f;
								if (particle)
								{
									particle->PositionInit(PlayerBoundBox.second->GetPosition(), otherPlayerBoundBox.second->GetLook(),REDCOLOR);
								}
							}
						}
					}
					else if (otherPlayerBoundBox.first == "rFoot" || otherPlayerBoundBox.first == "lFoot")
					{
						if (otherPlayerAnimation == ANIMATION_SIDE_KICK_L || otherPlayerAnimation == ANIMATION_1_2_3_KICK || otherPlayerAnimation == ANIMATION_KICK_MID_L)
						{
							if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == ATTACK && m_pPlayer->nowState == MIDDLE_GUARD)
							{
								m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_COME_HERE_1HAND);
							}
							else if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == ATTACK && m_pPlayer->nowState == OTHER)
							{
								continue;
							}
							else
							{
								m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_STRIGHT_B);
								if (effectManager)
								{
									effectManager->EffectOn(otherPlayerBoundBox.second->GetPosition(), 2);
								}
								m_pPlayer->hp += 0.08f;
								CoolTime = 0.f;
								CoolDown = false;
								effectTime = 0.f;
								if (particle)
								{
									particle->PositionInit(PlayerBoundBox.second->GetPosition(), otherPlayerBoundBox.second->GetLook(), REDCOLOR);
								}
							}
						}
					}
				}
				else if ((PlayerBoundBox.first == "lCalf" || PlayerBoundBox.first == "rCalf") &&
					(otherPlayerBoundBox.first == "rFoot" || otherPlayerBoundBox.first == "lFoot"))
				{
					// 하단 공격은 다리만 맞게 해야한다.
					if (otherPlayerAnimation == ANIMATION_LOW_KICK_R || otherPlayerAnimation == ANIMATION_LOW_KICK_SL || otherPlayerAnimation == ANIMATION_1_2_KICK||
						otherPlayerAnimation == ANIMATION_KICK_COMBO_HEAD)
					{
						if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == ATTACK && m_pPlayer->nowState == LOW_GUARD)
						{
							m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_COME_HERE_1HAND);
						}
						else if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == ATTACK && m_pPlayer->nowState == OTHER)
						{
							continue;
						}
						else
						{
							m_pPlayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_STRIGHT_B);
							if (effectManager)
							{
								effectManager->EffectOn(otherPlayerBoundBox.second->GetPosition(), REDX);
							}
							m_pPlayer->hp += 0.05f;
							CoolTime = 0.f;
							CoolDown = false;
							effectTime = 0.f;
							if (particle)
							{
								particle->PositionInit(PlayerBoundBox.second->GetPosition(), otherPlayerBoundBox.second->GetLook(), GREENCOLOR);
							}
						}
					}
				}
			}
		}
	}
}