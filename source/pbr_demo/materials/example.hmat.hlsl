/* The Halfling Project - A Graphics Engine and Projects
 *
 * The Halfling Project is the legal property of Adrian Astley
 * Copyright Adrian Astley 2013 - 2014
 */

#ifndef EXAMPLE_MATERIAL_SHADER_H
#define EXAMPLE_MATERIAL_SHADER_H

#define TEXTURE_COUNT 2

// The base shader will sample any supplied textures and call this function once for each pixel.
// Any outputs not explicitly set will stay at their default values.
//
// Default values:
// baseColor = (0.5f, 0.5f, 0.5f)
// specular = 0.5f
// normal = (0.0f, 0.0f, 1.0f)
// metallic = 0.0f
// roughness = 0.5f

void GetMaterialInfo(in float4 inputTextureSamples[TEXTURE_COUNT],
					 inout float3 baseColor, inout float specular, inout float3 normal, inout float metallic, inout float roughness) {
	// The first texture is base color and roughness
	baseColor = inputTextureSamples[0].rgb;
	roughness = lerp(0.4f, 0.8f, inputTextureSamples[0].a);

	// The second texture is the normal. There isn't an alpha channel
	normal = inputTextureSamples[1].rgb;

	// We'll leave the rest as default
}

#endif