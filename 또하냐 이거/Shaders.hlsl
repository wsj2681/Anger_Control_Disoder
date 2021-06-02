struct MATERIAL
{
	float4					m_cAmbient;
	float4					m_cDiffuse;
	float4					m_cSpecular; //a = power
	float4					m_cEmissive;
};

cbuffer cbCameraInfo : register(b1)
{
	matrix					gmtxView : packoffset(c0);
	matrix					gmtxProjection : packoffset(c4);
	float3					gvCameraPosition : packoffset(c8);
};

cbuffer cbGameObjectInfo : register(b2)
{
	matrix					gmtxGameObject : packoffset(c0);
	MATERIAL				gMaterial : packoffset(c4);
	uint					gnTexturesMask : packoffset(c8);
};

#include "Light.hlsl"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//#define _WITH_VERTEX_LIGHTING

#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40

Texture2D gtxtAlbedoTexture : register(t6);
Texture2D gtxtSpecularTexture : register(t7);
Texture2D gtxtNormalTexture : register(t8);
Texture2D gtxtMetallicTexture : register(t9);
Texture2D gtxtEmissionTexture : register(t10);
Texture2D gtxtDetailAlbedoTexture : register(t11);
Texture2D gtxtDetailNormalTexture : register(t12);

SamplerState gssWrap : register(s0);

struct VS_STANDARD_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

struct VS_STANDARD_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float3 tangentW : TANGENT;
	float3 bitangentW : BITANGENT;
	float2 uv : TEXCOORD;
};

VS_STANDARD_OUTPUT VSStandard(VS_STANDARD_INPUT input)
{
	VS_STANDARD_OUTPUT output;

	output.positionW = mul(float4(input.position, 1.0f), gmtxGameObject).xyz;
	output.normalW = mul(input.normal, (float3x3)gmtxGameObject);
	output.tangentW = mul(input.tangent, (float3x3)gmtxGameObject);
	output.bitangentW = mul(input.bitangent, (float3x3)gmtxGameObject);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return(output);
}

float4 PSStandard(VS_STANDARD_OUTPUT input) : SV_TARGET
{
	float4 cAlbedoColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	if (gnTexturesMask & MATERIAL_ALBEDO_MAP) cAlbedoColor = gtxtAlbedoTexture.Sample(gssWrap, input.uv);
	float4 cSpecularColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	if (gnTexturesMask & MATERIAL_SPECULAR_MAP) cSpecularColor = gtxtSpecularTexture.Sample(gssWrap, input.uv);
	float4 cNormalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	if (gnTexturesMask & MATERIAL_NORMAL_MAP) cNormalColor = gtxtNormalTexture.Sample(gssWrap, input.uv);
	float4 cMetallicColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	if (gnTexturesMask & MATERIAL_METALLIC_MAP) cMetallicColor = gtxtMetallicTexture.Sample(gssWrap, input.uv);
	float4 cEmissionColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	if (gnTexturesMask & MATERIAL_EMISSION_MAP) cEmissionColor = gtxtEmissionTexture.Sample(gssWrap, input.uv);

	float3 normalW;
	float4 cColor = cAlbedoColor + cSpecularColor + cMetallicColor + cEmissionColor;
	if (gnTexturesMask & MATERIAL_NORMAL_MAP)
	{
		float3x3 TBN = float3x3(normalize(input.tangentW), normalize(input.bitangentW), normalize(input.normalW));
		float3 vNormal = normalize(cNormalColor.rgb * 2.0f - 1.0f); //[0, 1] → [-1, 1]
		normalW = normalize(mul(vNormal, TBN));
	}
	else
	{
		normalW = normalize(input.normalW);
	}
	float4 cIllumination = Lighting(input.positionW, normalW);
	return(lerp(cColor, cIllumination, 0.5f));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define MAX_VERTEX_INFLUENCES			4
#define SKINNED_ANIMATION_BONES			128

cbuffer cbBoneOffsets : register(b7)
{
	float4x4 gpmtxBoneOffsets[SKINNED_ANIMATION_BONES];
};

cbuffer cbBoneTransforms : register(b8)
{
	float4x4 gpmtxBoneTransforms[SKINNED_ANIMATION_BONES];
};

struct VS_SKINNED_STANDARD_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	int4 indices : BONEINDEX;
	float4 weights : BONEWEIGHT;
};

VS_STANDARD_OUTPUT VSSkinnedAnimationStandard(VS_SKINNED_STANDARD_INPUT input)
{
	VS_STANDARD_OUTPUT output;

	//output.positionW = float3(0.0f, 0.0f, 0.0f);
	//output.normalW = float3(0.0f, 0.0f, 0.0f);
	//output.tangentW = float3(0.0f, 0.0f, 0.0f);
	//output.bitangentW = float3(0.0f, 0.0f, 0.0f);
	//matrix mtxVertexToBoneWorld;
	//for (int i = 0; i < MAX_VERTEX_INFLUENCES; i++)
	//{
	//	mtxVertexToBoneWorld = mul(gpmtxBoneOffsets[input.indices[i]], gpmtxBoneTransforms[input.indices[i]]);
	//	output.positionW += input.weights[i] * mul(float4(input.position, 1.0f), mtxVertexToBoneWorld).xyz;
	//	output.normalW += input.weights[i] * mul(input.normal, (float3x3)mtxVertexToBoneWorld);
	//	output.tangentW += input.weights[i] * mul(input.tangent, (float3x3)mtxVertexToBoneWorld);
	//	output.bitangentW += input.weights[i] * mul(input.bitangent, (float3x3)mtxVertexToBoneWorld);
	//}
	float4x4 mtxVertexToBoneWorld = (float4x4)0.0f;
	for (int i = 0; i < MAX_VERTEX_INFLUENCES; i++)
	{
//		mtxVertexToBoneWorld += input.weights[i] * gpmtxBoneTransforms[input.indices[i]];
		mtxVertexToBoneWorld += input.weights[i] * mul(gpmtxBoneOffsets[input.indices[i]], gpmtxBoneTransforms[input.indices[i]]);
	}
	output.positionW = mul(float4(input.position, 1.0f), mtxVertexToBoneWorld).xyz;
	output.normalW = mul(input.normal, (float3x3)mtxVertexToBoneWorld).xyz;
	output.tangentW = mul(input.tangent, (float3x3)mtxVertexToBoneWorld).xyz;
	output.bitangentW = mul(input.bitangent, (float3x3)mtxVertexToBoneWorld).xyz;

//	output.positionW = mul(float4(input.position, 1.0f), gmtxGameObject).xyz;

	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return(output);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtTerrainBaseTexture : register(t1);
Texture2D gtxtTerrainDetailTexture : register(t2);

struct VS_TERRAIN_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
};

struct VS_TERRAIN_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
};

VS_TERRAIN_OUTPUT VSTerrain(VS_TERRAIN_INPUT input)
{
	VS_TERRAIN_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.color = input.color;
	output.uv0 = input.uv0;
	output.uv1 = input.uv1;

	return(output);
}

float4 PSTerrain(VS_TERRAIN_OUTPUT input) : SV_TARGET
{
	float4 cBaseTexColor = gtxtTerrainBaseTexture.Sample(gssWrap, input.uv0);
	float4 cDetailTexColor = gtxtTerrainDetailTexture.Sample(gssWrap, input.uv1);
//	float4 cColor = saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));
	float4 cColor = input.color * saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));

	return(cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct VS_SKYBOX_CUBEMAP_INPUT
{
	float3 position : POSITION;
};

struct VS_SKYBOX_CUBEMAP_OUTPUT
{
	float3	positionL : POSITION;
	float4	position : SV_POSITION;
};

VS_SKYBOX_CUBEMAP_OUTPUT VSSkyBox(VS_SKYBOX_CUBEMAP_INPUT input)
{
	VS_SKYBOX_CUBEMAP_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.positionL = input.position;

	return(output);
}

TextureCube gtxtSkyCubeTexture : register(t13);
SamplerState gssClamp : register(s1);

float4 PSSkyBox(VS_SKYBOX_CUBEMAP_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtSkyCubeTexture.Sample(gssClamp, input.positionL);

	return(cColor);
}

struct VS_DIFFUSED_INPUT { 
	float3 position : POSITION;
	float4 color : COLOR;
};

//정점 셰이더의 출력(픽셀 셰이더의 입력)을 위한 구조체를 선언한다. 
struct VS_DIFFUSED_OUTPUT {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VS_DIFFUSED_OUTPUT VSDiffused(VS_DIFFUSED_INPUT input) {
	VS_DIFFUSED_OUTPUT output;
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.color = input.color;
	return(output);
}

//픽셀 셰이더를 정의한다.
float4 PSDiffused(VS_DIFFUSED_OUTPUT input) : SV_TARGET {
	return(input.color);
}


// 텍스쳐 UI
Texture2D gtxtUITexture : register(t14);
SamplerState gtxtUISampler : register(s2);

cbuffer cbHP_INFO : register(b3)
{
	float hp : packoffset(c0);
};

struct VS_TEXTURE_UI_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_TEXTURE_UI_OUTOUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_TEXTURE_UI_OUTOUT VSTextureUI(uint nVertexID : SV_VertexID)
{
	VS_TEXTURE_UI_OUTOUT output;
	float pos = .5f;
	if (nVertexID == 0) { output.position = float4(-pos, +pos, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(+pos, +pos, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(+pos, -pos, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(-pos, +pos, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(+pos, -pos, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(-pos, -pos, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }

	return output;
}

float4 PSTextureUI(VS_TEXTURE_UI_OUTOUT input) : SV_TARGET
{
	return gtxtUITexture.Sample(gtxtUISampler, input.uv);
}

VS_TEXTURE_UI_OUTOUT VSTextureUI_HP(uint nVertexID : SV_VertexID)
{
	VS_TEXTURE_UI_OUTOUT output;
	float x1 = -1.0f;
	float x2 = -0.2f / (hp / 100.f);
	float y1 = +1.0f;
	float y2 = +0.9f;
	if (nVertexID == 0) { output.position = float4(x1, y1, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(x2, y1, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(x2, y2, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(x1, y1, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(x2, y2, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(x1, y2, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }

	return output;
}

float4 PSTextureUI_HP(VS_TEXTURE_UI_OUTOUT input) : SV_TARGET
{
	return gtxtUITexture.Sample(gtxtUISampler, input.uv);
}

VS_TEXTURE_UI_OUTOUT VSTextureUI_HP2(uint nVertexID : SV_VertexID)
{
	VS_TEXTURE_UI_OUTOUT output;
	float x1 = +0.2f / (hp / 100.f);
	float x2 = +1.0f;
	float y1 = +1.0f;
	float y2 = +0.9f;
	if (nVertexID == 0) { output.position = float4(x1, y1, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(x2, y1, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(x2, y2, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(x1, y1, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(x2, y2, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(x1, y2, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }

	return output;
}

float4 PSTextureUI_HP2(VS_TEXTURE_UI_OUTOUT input) : SV_TARGET
{
	return gtxtUITexture.Sample(gtxtUISampler, input.uv);
}

VS_TEXTURE_UI_OUTOUT VSTextureUI_KeyRightShift(uint nVertexID : SV_VertexID)
{
	VS_TEXTURE_UI_OUTOUT output;
	float x1 = -1.0f;
	float x2 = -0.6f;
	float y1 = -0.4f;
	float y2 = -0.6f;
	if (nVertexID == 0) { output.position = float4(x1, y1, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(x2, y1, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(x2, y2, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(x1, y1, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(x2, y2, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(x1, y2, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }

	return output;
}

float4 PSTextureUI_KeyRightShift(VS_TEXTURE_UI_OUTOUT input) : SV_TARGET
{
	return gtxtUITexture.Sample(gtxtUISampler, input.uv);
}

VS_TEXTURE_UI_OUTOUT VSTextureUI_KeyLeftShift(uint nVertexID : SV_VertexID)
{
	VS_TEXTURE_UI_OUTOUT output;
	float x1 = +0.6f;
	float x2 = +1.0f;
	float y1 = -0.4f;
	float y2 = -0.6f;
	if (nVertexID == 0) { output.position = float4(x1, y1, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(x2, y1, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(x2, y2, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(x1, y1, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(x2, y2, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(x1, y2, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }

	return output;
}

float4 PSTextureUI_KeyLeftShift(VS_TEXTURE_UI_OUTOUT input) : SV_TARGET
{
	return gtxtUITexture.Sample(gtxtUISampler, input.uv);
}

VS_TEXTURE_UI_OUTOUT VSTextureUI_KeySpace(uint nVertexID : SV_VertexID)
{
	VS_TEXTURE_UI_OUTOUT output;
	float x1 = -0.3f;
	float x2 = +0.3f;
	float y1 = -0.8f;
	float y2 = -1.0f;
	if (nVertexID == 0) { output.position = float4(x1, y1, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 1) { output.position = float4(x2, y1, 0.0f, 1.0f); output.uv = float2(1.f, 0.f); }
	if (nVertexID == 2) { output.position = float4(x2, y2, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 3) { output.position = float4(x1, y1, 0.0f, 1.0f); output.uv = float2(0.f, 0.f); }
	if (nVertexID == 4) { output.position = float4(x2, y2, 0.0f, 1.0f); output.uv = float2(1.f, 1.f); }
	if (nVertexID == 5) { output.position = float4(x1, y2, 0.0f, 1.0f); output.uv = float2(0.f, 1.f); }

	return output;
}

float4 PSTextureUI_KeySpace(VS_TEXTURE_UI_OUTOUT input) : SV_TARGET
{
	return gtxtUITexture.Sample(gtxtUISampler, input.uv);
}