#shader [vertex]
#version 430 core


#define MAX_SPLIT 4

layout(std140, binding = 0) uniform Matrices
{
   mat4 view;
   mat4 proj; // offset 0
   mat4 model; // offset 16 * 4
   mat4 shadowSpaceMatrix[MAX_SPLIT]; //  128
};

void main()
{

};



#shader[fragment]
#version 430 core

#define MAX_SPLIT 4


//*** [Lighting] ***//

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

layout(std140, binding = 0) uniform Matrices
{
   mat4 view;
   mat4 proj; // offset 0
   mat4 model; // offset 16 * 4
   mat4 shadowSpaceMatrix[MAX_SPLIT]; //  128
};

layout(std140, binding = 1) uniform LightsUniform
{
    LightProperties Lights;
};

//
//layout(std140, binding = 2) uniform SSAOSamples
//{
//    vec3 samples[32];
//};
//
//
struct MaterialProperties
{
   int NormalEnabled;
    float Shininess;
    float SpecularHighlight;
    vec3 Diffuse;
    vec3 Ambient;
};

layout(std140, binding = 2) uniform MaterialUniform
{
    MaterialProperties Material;
};


out vec4 FragColor;


void main()
{
    FragColor = vec4(1.0);
};