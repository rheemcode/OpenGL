#shader [vertex]
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
} vs_out;


layout(std140, binding = 0) uniform Matrices
{
   mat4 view;
   mat4 proj; // offset 0
   mat4 model; // offset 16 * 4
   mat4 shadowSpaceMatrix[MAX_SPLIT]; //  128
};

void main()
{
    vec4 FragPos = model * vec4(vPos, 1.0);
    vs_out.FragPos = (view * FragPos).rgb;
    vs_out.TexCoord = texCoord;
    vs_out.NormalInterp = normalize((view * model * vec4(normal, 0.0))).xyz; 
    gl_Position = proj * view * FragPos;
};


#shader[fragment]
#version 430 core
//
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;


in VS_OUT
{
    vec2 TexCoord;
    vec3 FragPos;
    vec3 NormalInterp;
} vs_out;

uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;

void main()
{
    vec4 texDiff = texture(texture_diffuse1, vs_out.TexCoord);
    gPosition = vec4(vs_out.FragPos, gl_FragCoord.z);
    gNormal = vs_out.NormalInterp;
    gAlbedoSpec.rgb = texDiff.rgb;
 //   gAlbedoSpec.rgb = vec3(0.4, 0.4, 0.4);
    //gAlbedoSpec.a = texture(texture_specular1, vs_out.TexCoord).r;
};