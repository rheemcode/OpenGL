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

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D ssao;

//*** [Lighting] ***//

struct LightProperties
{
    int LightType;
    float AmbientEnergy;
    float Energy; 
    vec3 Ambient; 
    vec3 Color;
    vec3 Direction;
    //vec3 Position;
};

uniform vec3 ViewPosition;

layout(std140, binding = 0) uniform Matrices
{
   mat4 view;
   mat4 proj;
   mat4 model;
   mat4 shadowSpaceMatrix[4];
};


layout(std140, binding = 1) uniform LightsUniform
{
    LightProperties Lights;
};

void main()
{             
    vec4 FragPos = texture(gPosition, TexCoords);
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float AmbientOcclusion = texture(ssao, TexCoords).r;

    vec3 Color = Albedo; 
    vec3 ViewDir = normalize(ViewPosition - FragPos.rgb);

    vec3 LightDir = -Lights.Direction;

    float NdotL = max(dot(Normal, LightDir), 0.0);
    vec3 halfVec = normalize(LightDir + normalize(FragPos.rgb));
    float eyeLight = max(dot(Normal, halfVec), 0.0);
    float Specular = 0;

    if (NdotL > 0.0)
        Specular = pow(eyeLight, 2);

    vec3 Ambient = vec3(0.0) * 1.0;
    vec3 Diffuse = Lights.Color * NdotL * Lights.Energy * 1.0;
    vec3 Light = Ambient + Diffuse;

 
    Color = min(Light * Color, vec3(1));
    FragColor = vec4(Color, 1.0);
}  