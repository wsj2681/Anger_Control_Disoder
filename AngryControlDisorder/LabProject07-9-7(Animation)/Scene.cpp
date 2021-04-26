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

ID3D12DescriptorHeap *Scene::m_pd3dCbvSrvDescriptorHeap = NULL;

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

Scene::Scene()
{
	SoundManager::Init();

	soundManager = new SoundManager("Sound/ACD_mini.mid", true);
	
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
	*/
	m_nLights = lightsCount;
	m_nLights += 3;
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

	for (int i = 38 ; i < 40; ++i)
	{
		m_pLights[i].m_bEnable = true;
		m_pLights[i].m_nType = SPOT_LIGHT;
		m_pLights[i].m_fRange = 150.0f;
		m_pLights[i].m_xmf4Ambient = XMFLOAT4(0.1f, 0.f, 0.f, 1.0f);
		m_pLights[i].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.f, 0.f, 1.0f);
		m_pLights[i].m_xmf4Specular = XMFLOAT4(0.3f, 0.f, 0.f, 0.0f);
		m_pLights[i].m_xmf3Position = lights.data()[i]->GetPosition();
		m_pLights[i].m_xmf3Direction = XMFLOAT3(0.f, 60.f, -252.6f);
		m_pLights[i].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
		m_pLights[i].m_fFalloff = 8.0f;
		m_pLights[i].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
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
	m_pLights[40].m_fFalloff = 8.0f;
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
	m_pLights[41].m_fFalloff = 8.0f;
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
	m_pLights[42].m_fFalloff = 8.0f;
	m_pLights[42].m_fPhi = (float)cos(XMConvertToRadians(60.0f));
	m_pLights[42].m_fTheta = (float)cos(XMConvertToRadians(20.0f));
}

void Scene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);


	CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 76); //SuperCobra(17), Gunship(2), Player:Mi24(1), Angrybot()

	Material::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	
	m_pSkyBox = new SkyBox(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);

	ModelInfo* MapModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Arena_FBX.bin", NULL);
	Object* Map = new BoxerObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, MapModel, 1);
	Map->SetPosition(0.0f, 0.f, 0.0f);
	hierarchicalGameObjects.push_back(Map);
	if (MapModel) delete MapModel;

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
	ModelInfo* BoxerModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/ThaiBoxerR.bin", nullptr);
	Object* boxer = new BoxerObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, BoxerModel, 1);
	boxer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
	boxer->SetPosition(21.3046f, 10.0f, -769.689f);

	boxer->wayPoint.SetWayPoint(XMFLOAT3(21.3046f, 10.0f, -551.034f), ANIMATION_MOVE_FORWARD);
	boxer->wayPoint.SetWayPoint(XMFLOAT3(21.3046f, 1.66975f, -533.916f), ANIMATION_MOVE_FORWARD);
	boxer->wayPoint.SetWayPoint(XMFLOAT3(21.3046f, -5.69284f, -527.249f), ANIMATION_MOVE_FORWARD);
	boxer->wayPoint.SetWayPoint(XMFLOAT3(21.3046f, -5.69284f, -107.806f), ANIMATION_MOVE_FORWARD);

	boxer->wayPoint.SetWayPoint(XMFLOAT3(81.8642f, -5.69284f, -45.8827f), ANIMATION_CEREMONY);
	boxer->wayPoint.SetWayPoint(XMFLOAT3(79.623f, -5.69284f, 31.1354f), ANIMATION_CEREMONY);
	boxer->wayPoint.SetWayPoint(XMFLOAT3(37.5937f, -5.69284f, 80.0565f), ANIMATION_CEREMONY);
	boxer->wayPoint.SetWayPoint(XMFLOAT3(-29.7525f, -5.69284f, 81.7311f), ANIMATION_CEREMONY);
	boxer->wayPoint.SetWayPoint(XMFLOAT3(-77.2785f, -5.69284f, 41.0221f), ANIMATION_CEREMONY);
	boxer->wayPoint.SetWayPoint(XMFLOAT3(-81.0648f, -5.69284f, -29.1807f), ANIMATION_CEREMONY);
	boxer->wayPoint.SetWayPoint(XMFLOAT3(-17.0f, -5.69284f, -109.177f), ANIMATION_CEREMONY);
	boxer->wayPoint.SetWayPoint(XMFLOAT3(-17.0f, -5.69284f, -94.0986f), ANIMATION_MOVE_FORWARD);

	boxer->wayPoint.SetWayPoint(XMFLOAT3(-17.0f, 10.0f, -78.1817f), ANIMATION_MOVE_FORWARD);
	boxer->wayPoint.SetWayPoint(XMFLOAT3(-17.0f, 10.0f, -36.0f), ANIMATION_MOVE_FORWARD);
	boxer->wayPoint.SetWayPoint(XMFLOAT3(0.0f, 10.0f, -36.0f), ANIMATION_MOVE_FORWARD);

	hierarchicalGameObjects.push_back(boxer);
	if (BoxerModel) delete BoxerModel;

	ModelInfo* cubeModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Cube.bin", nullptr);
	Object* cube = new BoxerObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, cubeModel, 1);
	cube->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
	cube->isActive = false;
	cube->SetScale(1.5f, 1.5f, 1.5f);
	hierarchicalGameObjects.push_back(cube);

	ModelInfo* sphereModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/sphere.bin", nullptr);
	Object* sphere = new BoxerObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, sphereModel, 1);
	sphere->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
	sphere->isActive = false;
	sphere->SetScale(1.5f, 1.5f, 1.5f);
	hierarchicalGameObjects.push_back(sphere);

	if (cubeModel) delete cubeModel;

	ModelInfo* crowdModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Crowd.bin", NULL);

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
			//m_ppHierarchicalGameObjects[i]->SetPosition(0, 1.0f + 4.0f * (int)((i - 4) / 2), 130.0f + 12.5f * (i - 4));
			crowd->SetPosition(cos(XMConvertToRadians(angle)) * radius, 1.0f + 4.0f * i, sin(XMConvertToRadians(angle)) * radius);
			//crowd->Rotate(0.0f, angle + 90.f + ((j - nBaseModels) % (nCrowds - 1)) * 30.0f, 0.0f);
			crowd->Rotate(0.0f, 180.0f, 0.0f);
			hierarchicalGameObjects.push_back(crowd);
			angle += 30.f;
		}
		radius += 25.0f;
	}
	
	if (crowdModel) delete crowdModel;



	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void Scene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();
	if (m_pd3dCbvSrvDescriptorHeap) m_pd3dCbvSrvDescriptorHeap->Release();

	if (m_ppGameObjects)
	{
		for (int i = 0; i < m_nGameObjects; i++) if (m_ppGameObjects[i]) m_ppGameObjects[i]->Release();
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

	if (m_pSkyBox) delete m_pSkyBox;

	if (!hierarchicalGameObjects.empty())
	{
		for (auto& obj : hierarchicalGameObjects)
		{
			if (obj)
			{
				obj->Release();
			}
		}
		hierarchicalGameObjects.clear();
	}

	ReleaseShaderVariables();

	if (m_pLights) delete[] m_pLights;
}

ID3D12RootSignature *Scene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;

	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[11];

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

	D3D12_ROOT_PARAMETER pd3dRootParameters[17];

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

	D3D12_STATIC_SAMPLER_DESC pd3dSamplerDescs[2];

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

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = _countof(pd3dSamplerDescs);
	d3dRootSignatureDesc.pStaticSamplers = pd3dSamplerDescs;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}

void Scene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256의 배수
	m_pd3dcbLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);
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
		m_pd3dcbLights->Unmap(0, NULL);
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
			hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 1);
			break;
		case VK_F5:
			hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
			break;
		case VK_F6:
			hierarchicalGameObjects.data()[OTHERPLAYER]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 6);
			break;
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
		hierarchicalGameObjects.data()[1]->SetPosition(XMFLOAT3(21.3046f, 10.0f, -769.689f));
		hierarchicalGameObjects.data()[1]->wayPoint.SetCurWayPoints(0);
	}
	else
	{
		bScenario = false;
	}
}

bool Scene::ProcessInput(UCHAR *pKeysBuffer)
{
	return(false);
}

void Scene::AnimateObjects(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;

	for (int i = 0; i < m_nGameObjects; i++) if (m_ppGameObjects[i]) m_ppGameObjects[i]->Animate(fTimeElapsed);
	for (int i = 0; i < m_nShaders; i++) if (m_ppShaders[i]) m_ppShaders[i]->AnimateObjects(fTimeElapsed);
	if (bScenario) hierarchicalGameObjects[1]->UpdateWayPoints();

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

	// 충돌 할때 막은 상태라면 구를 렌더링 하고
	// 그게 아니고 맞은 상태라면 정육면체를 렌더링한다.

	if (m_pPlayer->rHand->isCollide)
	{
		// 플레이어-오른손공격 : 아더플레이어-왼손방어
		if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState != STATE_GUARD_LEFT_HEAD)
		{
			hierarchicalGameObjects.data()[CUBEOBJECT]->SetPosition(m_pPlayer->rHand->GetPosition());
			hierarchicalGameObjects.data()[CUBEOBJECT]->isActive = true;

		}
		else
		{
			hierarchicalGameObjects.data()[SPHEHROBJECT]->SetPosition(hierarchicalGameObjects.data()[OTHERPLAYER]->lHand->GetPosition());
			hierarchicalGameObjects.data()[SPHEHROBJECT]->isActive = true;
		}
	}
	else
	{
		//hierarchicalGameObjects.data()[CUBEOBJECT]->isActive = false;
		//hierarchicalGameObjects.data()[SPHEHROBJECT]->isActive = false;
	}
	if (m_pPlayer->lHand->isCollide)
	{
		// 플레이어-왼손공격 : 아더플레이어-오른손방어
		if (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState != STATE_GUARD_RIGHT_HEAD)
		{
			hierarchicalGameObjects.data()[CUBEOBJECT]->SetPosition(m_pPlayer->lHand->GetPosition());
			hierarchicalGameObjects.data()[CUBEOBJECT]->isActive = true;
		}
		else
		{
			hierarchicalGameObjects.data()[SPHEHROBJECT]->SetPosition(hierarchicalGameObjects.data()[OTHERPLAYER]->rHand->GetPosition());
			hierarchicalGameObjects.data()[SPHEHROBJECT]->isActive = true;
		}
	}
	else
	{
		//hierarchicalGameObjects.data()[CUBEOBJECT]->isActive = false;
		//hierarchicalGameObjects.data()[SPHEHROBJECT]->isActive = false;
	}
	if (m_pPlayer->head->isCollide)
	{
		// 플레이어-오른손방어 : 아더플레이어-왼손공격
		if (m_pPlayer->nowState == STATE_GUARD_RIGHT_HEAD && ((hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == STATE_ATTACK_LEFT_HOOK)||(hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == STATE_IDLE)))
		{
			hierarchicalGameObjects.data()[CUBEOBJECT]->SetPosition(m_pPlayer->head->GetPosition());
			hierarchicalGameObjects.data()[CUBEOBJECT]->isActive = true;
		}
	
		else if (m_pPlayer->nowState == STATE_GUARD_LEFT_HEAD && ((hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == STATE_ATTACK_RIGHT_HOOK) || (hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == STATE_IDLE)))
		{
			hierarchicalGameObjects.data()[CUBEOBJECT]->SetPosition(m_pPlayer->spine->GetPosition());
			hierarchicalGameObjects.data()[CUBEOBJECT]->isActive = true;
		}
		else// 공격을 제외한 상태일떄가 너무 많아서 맞으면 그냥 생김..
		{
			hierarchicalGameObjects.data()[SPHEHROBJECT]->SetPosition(m_pPlayer->lHand->GetPosition());
			hierarchicalGameObjects.data()[SPHEHROBJECT]->isActive = true;

		}
	}
	else
	{
		//hierarchicalGameObjects.data()[CUBEOBJECT]->isActive = false;
		//hierarchicalGameObjects.data()[SPHEHROBJECT]->isActive = false;
	}
	// 전면방어를 보여줄 공격
	//if (m_pPlayer->spine->isCollide)
	//{
	//	// 플레이어-전면방어 : 아더플레이어-아무손공격
	//	if (m_pPlayer->nowState == STATE_GUARD_BODY && hierarchicalGameObjects.data()[OTHERPLAYER]->nowState == STATE_ATTACK_RIGHT_HOOK)
	//	{
	//		hierarchicalGameObjects.data()[CUBEOBJECT]->SetPosition(m_pPlayer->spine->GetPosition());
	//		hierarchicalGameObjects.data()[CUBEOBJECT]->isActive = !hierarchicalGameObjects.data()[CUBEOBJECT]->isActive;
	//	}
	//}

	soundManager->Update();
}

