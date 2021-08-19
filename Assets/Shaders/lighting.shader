#shader vertex
#version 430 core


layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec2 TexCoord;
layout(location = 1) out vec3 FragPos;
layout(location = 2) out vec3 NormalInterp;
layout(location = 3) out vec4 ShadowCoord;
layout(location = 4) out vec3 Normal;

//uniform mat4 view;
//uniform mat4 proj;
uniform mat4 projView;
uniform mat4 model;
uniform mat4 shadowSpaceMatrix;
 
void main()
{
    ShadowCoord = shadowSpaceMatrix * (model * vec4(vPos, 1.0));
    Normal = transpose(inverse(mat3(model))) * normal;
    gl_Position = projView * model * vec4(vPos, 1.0);
    TexCoord = texCoord;
    NormalInterp = normalize((model * vec4(normal, 0.0)).xyz);
    FragPos = vec3(model * vec4(vPos, 1.0));
};



#shader fragment
#version 430 core

layout(location = 0) in vec2 TexCoord;
layout(location = 1) in vec3 FragPos;
layout(location = 2) in vec3 NormalInterp;
layout(location = 3) in vec4 ShadowCoord;
layout(location = 4) in vec3 Normal;

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
uniform sampler2D diffuseTexture[3];

uniform MaterialProperties Material;

out vec4 FragColor;


vec3 lightDir;
float attenuation;

float ShadowCalculation(vec4 ShadowPos)
{
   
    vec3 projCoords = ShadowPos.xyz / ShadowPos.w;
    float closestDepth = texture(depthTexture, projCoords.xy).r;
    float currentDepth = projCoords.z;
    //float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
    float bias = max(0.05 * (1.0 - dot(Normal, lightDir)), 0.005);
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    if (projCoords.z > 1.0)
        shadow = 0;

    return shadow;
}

void main()
{
    float attenuation;
    if (Lights.LightType == 1)
    {
        attenuation = 1;
    }



    lightDir = vec3(-Lights.Direction);

    float NdotL = max(dot(NormalInterp, lightDir), 0.0);
    vec3 halfVec = normalize(lightDir + normalize(-FragPos));
    float eyeLight = max(dot(NormalInterp, halfVec), 0.0);
    float Specular;
    if (NdotL > 0.0)
        Specular = 1 * pow(eyeLight, 2);
    else
        Specular = 0;

    vec3 Ambient = vec3(Lights.Ambient) * Lights.AmbientEnergy * attenuation;
    vec3 Diffuse = vec3(Lights.Color) * NdotL * Lights.Energy * attenuation;

    float shadow = ShadowCalculation(ShadowCoord);
    vec3 Light = Ambient + ((1.0 - shadow) *  Diffuse);
   // Light = (Ambient) + (Diffuse);

    //vec3 color = min(Light * vec3(texture(diffuseTexture[currentTex], TexCoord)), vec3(1.0));
    vec3 color = min(Light, vec3(1.0));
    FragColor = vec4(color, 1);
   // FragColor = vec4(vec3(texture(diffuseTexture[currentTex], TexCoord)), 1);
};