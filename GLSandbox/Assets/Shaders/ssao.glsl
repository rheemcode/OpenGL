#shader vertex
#version 430 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

#define MAX_SPLIT 4


void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}

#shader fragment
#version 430 core

#define USE_PCF

out float FragColor;
  
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform int kernelSize = 32;
uniform float radius = 1.0;
uniform float bias = 0.025;

layout(std140, binding = 0) uniform Matrices
{
   mat4 view;
   mat4 proj;
   mat4 model;
   mat4 shadowSpaceMatrix[4];
};

layout(std140, binding = 2) uniform SSAOSamples
{
    vec3 samples[32];
};

const vec2 noiseScale = vec2(1366.0/4.0, 768.0/4.0); 

void main()
{          
    
    vec3 FragPos  = texture(gPosition, TexCoords).xyz;
    vec3 Normal   = normalize(texture(gNormal, TexCoords).rgb);
    vec3 RandomVec = normalize(texture(texNoise, TexCoords * noiseScale).rgb);

    vec3 tangent = normalize(RandomVec - Normal * dot(RandomVec, Normal));
    vec3 bitangent = cross(Normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, Normal);

    float occlusion = 0.0;
    for (int i = 0; i < kernelSize; i++)
    {
        vec3 samplePos = TBN * samples[i];
        samplePos = FragPos + samplePos * radius;

        vec4 offset = vec4(samplePos, 1.0);
        offset = proj * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sampleDepth = texture(gPosition, offset.xy).z;
        
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(FragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;

    }
    
    occlusion = 1.0 - (occlusion / kernelSize);
    FragColor = occlusion;
}  