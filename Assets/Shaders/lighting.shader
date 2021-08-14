#shader vertex
#version 430 core


layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec2 TexCoord;
layout(location = 1) out vec3 FragPos;
layout(location = 2) out vec3 NormalInterp;
layout(location = 3) out vec4 ShadowCoord;

//uniform mat4 view;
//uniform mat4 proj;
uniform mat4 projView;
uniform mat4 model;
uniform mat4 shadowBias;
 
void main()
{
    ShadowCoord = shadowBias * (model * vec4(vPos, 1.0));
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

//*** [Lighting] ***//
uniform vec4  ViewPosition;
uniform float SpecularStrength;
uniform float Shininess;
uniform float AmbientEnergy;

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
uniform sampler2D diffuseTexture;
uniform sampler2D diffuseTexture2;
uniform sampler2D depthTexture;

uniform MaterialProperties Material;

out vec4 FragColor;


vec3 lightDir;
float attenuation;

float ShadowCalculation(vec4 ShadowPos)
{
    vec3 projCoords = ShadowPos.xyz / ShadowPos.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(depthTexture, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main()
{
    float attenuation;
    if (Lights.LightType == 1)
    {
        attenuation = 1;
    }
    

    vec3 lightDir = vec3(-Lights.Direction);

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
    vec3 Light = Ambient + ( Diffuse * (1.0));

    vec3 color = min(Light * vec3(texture(diffuseTexture2, TexCoord)), vec3(1.0));
    FragColor = vec4(color, 1);
};