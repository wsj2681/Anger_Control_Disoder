cbuffer cbGameObjectInfo : register(b1)
{
	matrix gameObject : packoffset(c0);
}

cbuffer cbCameraInfo : register(b2)
{
	matrix	gmtxView : packoffset(c0);
	matrix	gmtxProjection : packoffset(c4);
}

struct VS_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
};

VS_OUTPUT VSCube(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = mul(mul(mul(float4(input.position, 1.0f), gameObject), gmtxView), gmtxProjection);
	output.color = input.color;
	return output;
}

float4 PSCube(VS_OUTPUT input) : SV_TARGET
{
	return input.color;
}