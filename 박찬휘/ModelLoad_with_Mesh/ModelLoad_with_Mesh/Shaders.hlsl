cbuffer cbCameraInfo : register(b1)
{
	matrix					gmtxView : packoffset(c0);
	matrix					gmtxProjection : packoffset(c4);
	float3					gvCameraPosition : packoffset(c8);
};

cbuffer cbGameObjectInfo : register(b2)
{
	matrix 					gmtxGameObject : packoffset(c0);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct VS_FBX_MODEL_INPUT
{
	float4 position : POSITION;
};

struct VS_FBX_MODEL_OUTPUT
{
	float4	position : SV_POSITION;
};

VS_FBX_MODEL_OUTPUT VSFbxModel(VS_FBX_MODEL_INPUT input)
{
	VS_FBX_MODEL_OUTPUT output;

	output.position = mul(mul(mul(input.position, gmtxGameObject), gmtxView), gmtxProjection);

	return(output);
}

float4 PSFbxModel(VS_FBX_MODEL_OUTPUT input) : SV_TARGET
{	
	float4 cColor = float4(1.0f, 0.0f, 0.0f, 1.0f);

	return(cColor);
}

VS_FBX_MODEL_OUTPUT VSFbxSkinnedModel(VS_FBX_MODEL_INPUT input)
{
	VS_FBX_MODEL_OUTPUT output;

	output.position = mul(mul(mul(input.position, gmtxGameObject), gmtxView), gmtxProjection);

	return(output);
}

float4 PSFbxSkinnedModel(VS_FBX_MODEL_OUTPUT input) : SV_TARGET
{	
	float4 cColor = float4(0.0f, 0.0f, 1.0f, 1.0f);

	return(cColor);
}
