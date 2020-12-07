cbuffer mapInfo : register(b0)
{
	matrix		world : packoffset(c0);
}

cbuffer cameraInfo : register(b1)
{
	matrix		view : packoffset(c0);
	matrix		projection : packoffset(c4);
	matrix		viewProjection : packoffset(c8);
	float3		cameraPosition : packoffset(c12);
}

Texture2D<float4> terrainBaseTexture : register(t0);
Texture2D<float4> terrainDetailTexture : register(t1);
Texture2D<float4> terrainAlphaTexture : register(t2);

SamplerState wrapSamplerState : register(s0);
SamplerState clampSamplerState : register(s1);

struct VS_TERRAIN_INPUT
{
	float3 position : POSITION;`
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
};

struct VS_TERRAIN_OUTPUT
{
	float3 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
};

VS_TERRAIN_OUTPUT VSTerrain(VS_TERRAIN_INPUT input)
{
	VS_TERRAIN_OUTPUT output;

	output.position = mul(mulmul((float4(input.position, 1.f), world), view), projection);
	output.color = input.color;
	output.uv0 = input.uv0;
	output.uv1 = input.uv1;

	return output;
}

float4 PSTerrain(VS_TERRAIN_OUTPUT input) : SV_TARGET
{
	float4 baseTexColor = terrainBaseTexture.Sample(wrapSamplerState, input.uv0);
	float4 detailTexColor = terrainDetailTexture.Sample(wrapSamplerState, input.uv1);
	float fAlpha = terrainAlphaTexture.Sample(wrapSamplerState, input.uv0);

	float4 color = saturate(lerp(baseTexColor, detailTexColor, fAlpha));

	return color;
}

struct VS_TERRAIN_TESSELLATION_OUTPUT
{
	float3 position : POSITION;
	float3 positionW : POSITION1;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
};

VS_TERRAIN_TESSELLATION_OUTPUT VSTerrainTessellation(VS_TERRAIN_INPUT input)
{
	VS_TERRAIN_TESSELLATION_OUTPUT output;

	output.position = input.position;
	output.positionW = mul(float4(input.position, 1.f), world).xyz;
	output.uv0 = input.uv0;
	output.uv1 = input.uv1;

	return output;
}

struct HS_TERRAIN_TESSELLATION_CONSTANT
{
	float fTessEdges[4] : SV_TessFactor;
	float fTessInsides[2] : SV_InsideTessFactor;
};

struct HS_TERRAIN_TESSELLATION_OUTPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
};

struct DS_TERRAIN_TESSELLATION_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float4 tessellation : TEXCOORD2;
};

void BernsteinCoeffcient5x5(float t, out float fBernstein[5])
{
	float tInv = 1.f - t;
	fBernstein[0] = tInv * tInv * tInv * tInv;
	fBernstein[1] = 4.f * t * tInv * tInv * tInv;
	fBernstein[2] = 6.f * t * t * tInv * tInv;
	fBernstein[3] = 4.f * t * t * t * tInv;
	fBernstein[4] = t * t * t * t;
}

float3 CubicBezierSum5x5(OutputPatch<HS_TERRAIN_TESSELLATION_OUTPUT, 25> patch, float uB[5], float vB[5])
{

}