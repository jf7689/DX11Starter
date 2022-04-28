#include "ShaderIncludes.hlsli"

cbuffer ExternalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
}

struct VertexToPixel_Shadow
{
	float4 screenPosition	: SV_POSITION;
}; 

VertexToPixel_Shadow main(VertexShaderInput input)
{
	// Set up output struct
	VertexToPixel_Shadow output;

	matrix wvp = mul(mul(projection, view), world);
	output.screenPosition = mul(wvp, float4(input.localPosition, 1.0f));

	return output;
}