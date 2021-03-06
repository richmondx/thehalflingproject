/* The Halfling Project - A Graphics Engine and Projects
 *
 * The Halfling Project is the legal property of Adrian Astley
 * Copyright Adrian Astley 2013
 */

#include "types.hlsli"
#include "common/shaders/materials.hlsli"
#include "common/shaders/lights.hlsli"
#include "common/shaders/light_functions.hlsli"


cbuffer cbPerFrame : register(b2) {
	DirectionalLight gDirectionalLight;
	float3 gEyePosition;
}

cbuffer cbPerObject : register(b3) {
	Material gMaterial;
};

StructuredBuffer<PointLight> gPointLights : register(t4);
StructuredBuffer<SpotLight> gSpotLights : register(t5);


float4 PS(PixelIn input) : SV_TARGET {
	// Interpolating can unnormalize
	input.normal = normalize(input.normal);
	input.positionWorld = normalize(input.positionWorld);

	float3 toEye = normalize(gEyePosition - input.positionWorld);

	// Initialize
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sum the contribution from each source

	uint numLights, dummy, lightIndex;

	AccumulateDirectionalLight(gMaterial, gDirectionalLight, input.normal, toEye, ambient, diffuse, spec);

	gPointLights.GetDimensions(numLights, dummy);
	for (lightIndex = 0; lightIndex < numLights; ++lightIndex) {
        PointLight light = gPointLights[lightIndex];
		AccumulatePointLight(gMaterial, light, input.positionWorld, input.normal, toEye, ambient, diffuse, spec);
    }

	gSpotLights.GetDimensions(numLights, dummy);
	for (lightIndex = 0; lightIndex < numLights; ++lightIndex) {
        SpotLight light = gSpotLights[lightIndex];
		AccumulateSpotLight(gMaterial, light, input.positionWorld, input.normal, toEye, ambient, diffuse, spec);
    }

	float4 litColor = ambient + diffuse + spec;

	// Take alpha from diffuse material
	litColor.a = gMaterial.Diffuse.a;

	return litColor;
}