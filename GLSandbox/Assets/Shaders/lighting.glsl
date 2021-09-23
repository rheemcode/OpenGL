//#shader [vertex]
#version 430 core


layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
#define MAX_SPLIT 4

out VS_OUT
{
    vec2 TexCoord;
    vec3 FragPos;
    vec3 NormalInterp;
    vec4 ShadowCoord[MAX_SPLIT];
    vec3 Normal;
} vs_out;


layout(std140, binding=0) uniform Matrices
{
   mat4 projView; // offset 0
   mat4 model; // offset 16 * 4
   mat4 shadowSpaceMatrix[MAX_SPLIT]; //  128
};

void main()
{
    vec4 FragPos = model * vec4(vPos, 1.0);
    vs_out.FragPos = vec3(FragPos);

    for (int i = 0; i < MAX_SPLIT; i++)
        vs_out.ShadowCoord[i] = shadowSpaceMatrix[i] * FragPos;

    vs_out.Normal = transpose(inverse(mat3(model))) * normal;
    vs_out.TexCoord = texCoord;
    vs_out.NormalInterp = normalize((model * vec4(normal, 0.0)).xyz);
    
    gl_Position = projView * FragPos;
};



#shader[fragment]
#version 430 core


#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2
#define POINT_LIGHT 3


in VS_OUT
{
    vec2 TexCoord;
    vec3 FragPos;
    vec3 NormalInterp;
    vec4 ShadowCoord[4];
    vec3 Normal;
} vs_out;


//*** [Lighting] ***//
uniform int currentTex;

struct LightProperties
{
    int LightType; // 0
    float AmbientEnergy; // 4
    float Energy; // 8
    vec3 Ambient; // 16
    vec3 Color; // 32
    vec3 Direction; // 48
    //vec3 Position;
};


struct MaterialProperties
{
    int diffuseMap;
    vec4 Color;
    float Shininess;
    float SpecularHighlights;
};

layout(std140, binding=1) uniform LightsUniform
{
    LightProperties Lights;
};

uniform sampler2DArray depthTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform vec4 farDistance;

uniform MaterialProperties Material;

out vec4 FragColor;


vec3 lightDir;
float attenuation;

const int pcfCount = 2;
const float totalTexels = (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);
const float mapSize = 2048.0 * 2.0;
const float texelSize = 1.0 / mapSize;

#define VEC3   vec3(0, 0, 0)
#define VEC3_1 vec3(1, 1, 1)
#define ZERO 0
#define ONE 1.0

const vec2 poissonDisk[8] = { vec4(0.000000, 0.000000, 0.0, 0.0),
								  vec4(0.079821, 0.165750, 0.0, 0.0),
								  vec4(-0.331500, 0.159642, 0.0, 0.0),
								  vec4(-0.239463, -0.497250, 0.0, 0.0),
								  vec4(0.662999, -0.319284, 0.0, 0.0),
								  vec4(0.399104, 0.828749, 0.0, 0.0),
								  vec4(-0.994499, 0.478925, 0.0, 0.0),
								  vec4(-0.558746, -1.160249, 0.0, 0.0) };

float random(vec3 seed, int i) {
    vec4 seed4 = vec4(seed, i);
    float dot_product = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));
    return fract(sin(dot_product) * 43758.5453);
}

int getDepth()
{
    int index = 3;

    if (gl_FragCoord.z < farDistance.x)
        index = 0;
    else if (gl_FragCoord.z < farDistance.y)
        index = 1;
    else if (gl_FragCoord.z < farDistance.z)
        index = 2;

    return index;
}

float ShadowCalculation(vec4 ShadowPos, int index)
{

    vec3 projCoords = ShadowPos.xyz / ShadowPos.w;
    //projCoords = clamp(projCoords, VEC3, VEC3_1);
    if (projCoords.z > || projCoords.z < 0)
        return 0;

    float currentDepth = projCoords.z;
    float bias = max(0.002 * (ONE - dot(vs_out.Normal, lightDir)), 0.002);
    float shadow = 0.0;
    int index = 0;
  //  vec2 offset;
    for (int x = -pcfCount; x <= pcfCount; ++x)
    {
        for (int y = -pcfCount; y <= pcfCount; ++y)
        {
        //    int index = int(16.0 * random(floor(vs_out.FragPos.xyz * 1000.0), x + y * 4)) % 16;
            index++;
        //    offset.x = x; offset.y = y;
            float pcfDepth = texture(depthTexture, vec3(projCoords.xy + poissonDisk[index] * 2.0, index)).x;
            if (currentDepth  > pcfDepth + bias)
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
 //   if (texCol.a == 0)
   //     discard;

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

    int shadowIndex = getDepth();
    float shadow = ShadowCalculation(vs_out.ShadowCoord[shadowIndex], shadowIndex);
    vec3 Light = Ambient + ((ONE - (shadow * vs_out.ShadowCoord[shadowIndex].w)) * Diffuse);
    //vec3 Light = (Ambient) + (Diffuse);

   // vec3 color = min(Light * vec3(texCol), VEC3_1);
    vec3 color = min(Light, VEC3_1);
    FragColor = vec4(color, texCol.a);

//    FragColor = vec4(vec3(texCol), 1);
};