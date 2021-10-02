#shader [compute]
#version 430 core

layout (local_size_x = 8, local_size_y = 8) in;

layout (r8, binding = 0) uniform restrict writeonly image2D ssao_image;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform int kernelSize = 16;
uniform float radius = .5;
uniform float bias = 0.025;
uniform vec2 screenSize = vec2(1366, 768);

layout(std140, binding = 0) uniform Matrices
{
   mat4 view;
   mat4 proj;
   mat4 model;
   mat4 shadowSpaceMatrix[4];
};

layout(std140, binding = 2) uniform SSAOSamples
{
    vec3 samples[16];
};

const vec2 noiseScale = screenSize / 4; 
const vec2 dimension = textureSize(gPosition, 0);

void main()
{
	ivec2 PixelCoords = ivec2(gl_GlobalInvocationID.xy);
	vec2 TexCoords = (PixelCoords) / screenSize;

	vec3 FragPos = texture(gPosition, TexCoords).xyz;
	vec3 Normal  = normalize(texture(gNormal, TexCoords).xyz);
	vec3 RandomVec = normalize(texture(texNoise, TexCoords).xyz);

	vec3 Tangent = normalize(RandomVec - Normal * dot(RandomVec, Normal));
	vec3 BiTangent = cross(Normal, Tangent);
	mat3 TBN = mat3(Tangent, BiTangent, Normal);

	float occlusion = 0.0;
	for (int i = 0; i < kernelSize; ++i)
	{
		vec3 samplePos = TBN * samples[i];
		samplePos = FragPos + samplePos * radius;

		vec4 offset = vec4(samplePos, 1.0);
		offset = proj * offset;
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5; // mapping to 0 - 1

		float sampleDepth = texture(gPosition, offset.xy).z;
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(FragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
	}

	occlusion = 1.0 - (occlusion / kernelSize);
	imageStore(ssao_image, PixelCoords, vec4(occlusion));
}