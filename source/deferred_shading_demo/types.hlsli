/* The Halfling Project - A Graphics Engine and Projects
 *
 * The Halfling Project is the legal property of Adrian Astley
 * Copyright Adrian Astley 2013
 */

#ifndef DEFERRED_SHADING_DEMO_TYPES_H
#define DEFERRED_SHADING_DEMO_TYPES_H

struct VertexIn {
	float3 position  : POSITION;
	float3 normal    : NORMAL;
	float2 texCoord  : TEXCOORD;
};

struct ForwardPixelIn {
	float4 positionClip   : SV_POSITION;
	float3 positionWorld  : POSITION_WORLD;
	float3 normal         : NORMAL;
	float2 texCoord       : TEXCOORD;
};

struct GBufferShaderPixelIn {
	float4 positionClip   : SV_POSITION;
	float3 normal         : NORMAL;
	float2 texCoord       : TEXCOORD;
};

struct FullScreenTrianglePixelIn {
	float4 positionClip  : SV_POSITION;
};

struct TransformedFullScreenTrianglePixelIn {
	float4 positionClip  : SV_POSITION;
	float2 texCoord      : TEXCOORD;
};

struct DebugObjectShaderVertexIn {
	float3 position                   : POSITION;
	float4x4 instanceWorldViewProj    : INSTANCE_WORLDVIEWPROJ;
	float4 instanceColor              : INSTANCE_COLOR;
};

struct DebugObjectShaderPixelIn {
	float4 positionClip   : SV_POSITION;
	float4 color          : COLOR;
};

#endif
