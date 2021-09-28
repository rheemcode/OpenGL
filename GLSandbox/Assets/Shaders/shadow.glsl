//#shader [vertex]
#version 430 core

#extension EXT_gpu_shader4 : require

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 texCoords;

#define MAX_SPLIT 4

layout(std140, binding = 0) uniform Matrices
{
	mat4 projView;
	mat4 model;
	mat4 shadowSpaceMatrix[MAX_SPLIT];
};

out VS_OUT
{
	vec3 FragPos;
	vec3 NormalInterp;
	vec3 ShadowCoord[MAX_SPLIT];
} vs_out;

void main()
{
	vec4 FragPos = model * vec4(vPos, 1.0);

	for (int i = 0; i < MAX_SPLIT; i++)
	{
		vs_out.ShadowCoord[i] = shadowSpaceMatrix[i] * FragPos;
	}

	vs_out.Normal = normalize((model * vec4(normal, 0.0)).xyz);
	gl_Position = projView * FragPos;
}

#shader [fragment]
#version 430 core

#define MAX_SPLIT 4

in VS_OUT
{
	vec3 FragPos;
	vec3 NormalInterp;
	vec3 ShadowCoord[MAX_SPLIT];
} fs_in;

uniform int samples = 4;
uniform vec2 depthMapSize = vec2(2048, 2048);
const float totalTexels = (samples * 2.0 + 1.0) * (samples * 2.0 + 1.0);
const vec2 texelSize = 1.0 / depthMapSize;

out vec4 FragColor;

int get_d

void main()
{
	
}