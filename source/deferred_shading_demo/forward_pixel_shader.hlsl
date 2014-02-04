/* The Halfling Project - A Graphics Engine and Projects
 *
 * The Halfling Project is the legal property of Adrian Astley
 * Copyright Adrian Astley 2013
 */

#include "types.hlsli"
#include "common/shaders/materials.hlsli"
#include "common/shaders/lights.hlsli"
#include "common/shaders/light_functions.hlsli"


cbuffer cbPerFrame : register(b0) {
	DirectionalLight gDirectionalLight;
	float3 gEyePosition;
}

cbuffer cbPerObject : register(b1) {
	BlinnPhongMaterial gMaterial;
};

Texture2D gDiffuseTexture : register(t0);
SamplerState gDiffuseSampler : register(s0);

StructuredBuffer<PointLight> gPointLights : register(t3);
StructuredBuffer<SpotLight> gSpotLights : register(t4);


float4 ForwardPS(ForwardPixelIn input) : SV_TARGET {
	// Interpolating can unnormalize
	input.normal = normalize(input.normal);

	float3 toEye = normalize(gEyePosition - input.positionWorld);

	// Initialize
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sample the texture
	float4 textureColor = gDiffuseTexture.Sample(gDiffuseSampler, input.texCoord);

	// Sum the contribution from each light source
	uint numLights, lightIndex, dummy;

	AccumulateBlinnPhongDirectionalLight(gMaterial, gDirectionalLight, input.normal, toEye, ambient, diffuse, spec);

	gPointLights.GetDimensions(numLights, dummy);
	for (lightIndex = 0; lightIndex < numLights; ++lightIndex) {
		PointLight light = gPointLights[lightIndex];
		AccumulateBlinnPhongPointLight(gMaterial, light, input.positionWorld, input.normal, toEye, diffuse, spec);
	}

	gSpotLights.GetDimensions(numLights, dummy);
	for (lightIndex = 0; lightIndex < numLights; ++lightIndex) {
		SpotLight light = gSpotLights[lightIndex];
		AccumulateBlinnPhongSpotLight(gMaterial, light, input.positionWorld, input.normal, toEye, diffuse, spec);
	}

	// Combine
	float4 litColor = textureColor * (ambient + diffuse) + spec;

	// Take alpha from diffuse material
	litColor.a = gMaterial.Diffuse.a;

	return litColor;
}