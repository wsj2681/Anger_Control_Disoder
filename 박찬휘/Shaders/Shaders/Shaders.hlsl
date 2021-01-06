cbuffer cbCameraInfo: register(b1)
{
	matrix mView : packoffset(c0);
	matrix mProjection : packoffset(c4);
	float3 fCameraPosition : packoffset(c8);
}

// gameobjectinfo b2

#include "Light.hlsl"

SamplerState wrapSamplerState : register(s0);

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
	float3 position : SV_POSITION;
	float3 positionW : POSITION;
	float2 uv : TEXCOORD;
	float3 normalW : NORMAL;
	float3 tangentW : TANGENT;
	float3 bitangentW : BITANGENT;
};

VS_STANDARD_OUTPUT VSStandard(VS_STANDARD_INPUT input)
{
	VS_STANDARD_OUTPUT output;

	output.position = mul(mul(float4(output.positionW, 1.0f), view), projection);
	output.positionW = mul(float4(input.position, 1.0f), gameObject).xyz;
	output.uv = input.uv;
	output.normalW = mul(input.normal, (float3x3)gameObject);
	output.tangentW = mul(input.tangent, (float3x3)gameObject);
	output.bitangentW = mul(input.bitangent, (float3x3)gameObject);
	
	return output;
}

//float4 PSStandard(VS_STANDARD_OUTPUT input) : SV_TARGET
//{
//	float4 fAlbedoColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
//
//	float3 normalW;
//	float4 color;
//
//	return 
//}