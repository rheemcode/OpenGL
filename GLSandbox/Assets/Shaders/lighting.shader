#shader [vertex]
#version 430 core


layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VS_OUT
{
    vec2 TexCoord;
    vec3 FragPos;
    vec3 NormalInterp;
    vec4 ShadowCoord;
    vec3 Normal;
} vs_out;



//uniform mat4 view;
//uniform mat4 proj;
uniform mat4 projView;
uniform mat4 model;
uniform mat4 shadowSpaceMatrix;

void main()
{

    vs_out.ShadowCoord = shadowSpaceMatrix * (model * vec4(vPos, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * normal;
    vs_out.TexCoord = texCoord;
    vs_out.NormalInterp = normalize((model * vec4(normal, 0.0)).xyz);
    vs_out.FragPos = vec3(model * vec4(vPos, 1.0));
    
    gl_Position = projView * model * vec4(vPos, 1.0);
};



#shader[fragment]
#version 430 core

in VS_OUT
{
    vec2 TexCoord;
    vec3 FragPos;
    vec3 NormalInterp;
    vec4 ShadowCoord;
    vec3 Normal;
} vs_out;


//*** [Lighting] ***//
uniform vec4  ViewPosition;
uniform float SpecularStrength;
uniform float Shininess;
uniform float AmbientEnergy;
uniform int currentTex;

struct LightProperties
{
    int LightType;
    vec4 Ambient;
    vec4 Color;
    vec4 Direction;
    float AmbientEnergy;
    float Energy;
};

struct MaterialProperties
{
    int diffuseMap;
    vec4 Color;
    float Shininess;
    float SpecularHighlights;

};

layout(std140) uniform LightsUniform
{
    LightProperties Lights;
};
uniform sampler2D depthTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;


uniform MaterialProperties Material;

out vec4 FragColor;


vec3 lightDir;
float attenuation;

const int pcfCount = 2;
const float totalTexels = (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);
const float mapSize = 2048.0 * 2.0;
const float texelSize = 1.0 / mapSize;

#define VEC3 vec3(0, 0, 0)
#define VEC3_1 vec3(1, 1, 1)
#define ZERO 0
#define ONE 1.0

float ShadowCalculation(vec4 ShadowPos)
{

    vec3 projCoords = ShadowPos.xyz / ShadowPos.w;
    //projCoords = clamp(projCoords, VEC3, VEC3_1);
    if (projCoords.z > ONE || projCoords.z < ZERO || projCoords.x < ZERO || projCoords.x > ONE || projCoords.y < ZERO)
        return 0;

    float currentDepth = projCoords.z;
   // float bias = max(0.01 * (ONE - dot(vs_out.Normal, lightDir)), 0.001);
    float shadow = 0.0;

    vec2 offset;
    for (int x = -pcfCount; x <= pcfCount; ++x)
    {
        for (int y = -pcfCount; y <= pcfCount; ++y)
        {
            offset.x = x; offset.y = y;
            float pcfDepth = texture(depthTexture, projCoords.xy + offset * texelSize).x;
            if (currentDepth  > pcfDepth + 0.002)
                shadow += ONE;
            //shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= totalTexels;
    return shadow;
}

void main()
{
    vec4 texCol = texture(diffuseTexture, vs_out.TexCoord);
    //if (texCol.a == 0)
     //   discard;

    float attenuation;
    if (Lights.LightType == 1)
    {
        attenuation = ONE;
    }



    lightDir = vec3(-Lights.Direction);

    float NdotL = max(dot(vs_out.NormalInterp, lightDir),ZERO);
    vec3 halfVec = normalize(lightDir + normalize(-vs_out.FragPos));
    float eyeLight = max(dot(vs_out.NormalInterp, halfVec), ZERO);
    float Specular;
    if (NdotL > ZERO)
        Specular = ONE * pow(eyeLight, 2);
    else
        Specular = ZERO;

    vec3 Ambient = vec3(Lights.Ambient) * Lights.AmbientEnergy * attenuation;
    vec3 Diffuse = vec3(Lights.Color) * NdotL * Lights.Energy * attenuation;

    float shadow = ShadowCalculation(vs_out.ShadowCoord);
    vec3 Light = Ambient + ((ONE - (shadow * vs_out.ShadowCoord.w)) * Diffuse);
    // Light = (Ambient) + (Diffuse);

    vec3 color = min(Light * vec3(texCol), VEC3_1);
    //vec3 color = min(Light, VEC3_1);
    FragColor = vec4(color, texCol.a);

//    FragColor = vec4(vec3(texCol), 1);
};