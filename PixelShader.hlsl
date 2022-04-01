#include "ShaderIncludes.hlsli"

cbuffer ExternalData : register(b0)
{
	float4 colorTint;
	float3 cameraPosition;
	float roughness;
	float3 cameraPos;
	float3 ambientLight;
	float uvScale;
	float2 uvOffset;
	Light dirLight1;
	Light dirLight2;
	Light dirLight3;
	Light pointLight1;
	Light pointLight2;
}

Texture2D SurfaceTexture	: register(t0);
SamplerState BasicSampler	: register(s0);

float3 Attenuate(Light light, float3 worldPos)
{
	float dist = distance(light.Position, worldPos);
	float attenuate = saturate(1.0f - (dist * dist / (light.Range * light.Range)));
	return attenuate * attenuate;
}


float3 CalculateDirectionalLight(Light light, VertexToPixel inputData)
{
	// Normalized direction to the light
	float3 dirToLight = normalize(-light.Direction);

	// Diffuse Amount
	float diffuseAmount = saturate(dot(inputData.normal, dirToLight));

	// Specular for light
	float specExponent = (1.0f - roughness) * MAX_SPECULAR_EXPONENT;
	float specular;

	if (specExponent > 0.05f)
	{
		float3 viewVectorToCam = normalize(cameraPosition - inputData.worldPosition);
		float3 reflVector = reflect(-dirToLight, inputData.normal);
		specular = pow(saturate(dot(reflVector, viewVectorToCam)), specExponent);
	}

	// Final color
	float3 lightColor = ((diffuseAmount * light.Color * colorTint) + (ambientLight * colorTint) + specular) * light.Intensity;
	return lightColor;
}


float3 CalculatePointLight(Light light, VertexToPixel inputData)
{
	// Normalized direction to the light
	float3 dirToLight = normalize(light.Position - inputData.worldPosition);

	// Diffuse Amount
	float diffuseAmount = saturate(dot(inputData.normal, dirToLight));

	// Specular for light
	float specExponent = (1.0f - roughness) * MAX_SPECULAR_EXPONENT;
	float specular;

	if (specExponent > 0.05f)
	{
		float3 viewVectorToCam = normalize(cameraPosition - inputData.worldPosition);
		float3 reflVector = reflect(-dirToLight, inputData.normal);
		specular = pow(saturate(dot(reflVector, viewVectorToCam)), specExponent);
	}

	// Final color
	float3 lightColor = ((diffuseAmount * light.Color * colorTint) + (ambientLight * colorTint) + specular) * Attenuate(light, inputData.worldPosition) * light.Intensity;
	return lightColor;
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);

	// Scale/Offseet the uvs of the texture
	input.uv = (input.uv + uvOffset) * uvScale;

	// Set texture color
	float3 surfaceColor = SurfaceTexture.Sample(BasicSampler, input.uv).rgb;

	// Tint surface color
	surfaceColor = surfaceColor * colorTint;

	float3 finalColor = surfaceColor + CalculateDirectionalLight(dirLight1, input) + CalculateDirectionalLight(dirLight2, input) + CalculateDirectionalLight(dirLight3, input);
	finalColor += CalculatePointLight(pointLight1, input) + CalculatePointLight(pointLight2, input);

	return float4(finalColor, 1);
}