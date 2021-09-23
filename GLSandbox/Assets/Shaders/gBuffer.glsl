#shader [vertex]
#version 410 core


layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;


#define MAX_SPLIT 4

out VS_OUT
{
    vec2 TexCoord;
    vec3 FragPos;
    vec3 NormalInterp;
  //  vec3 Normal;
} vs_out;


//uniform mat4 view;
//uniform mat4 proj;
uniform mat4 projView; // offset 0
uniform mat4 model; // offset 16 * 4//  128

void main()
{
    vec4 FragPos = model * vec4(vPos, 1.0);
    vs_out.FragPos = vec3(FragPos);
//
  //  vs_out.Normal = transpose(inverse(mat3(model))) * normal;
    vs_out.TexCoord = texCoord;
    vs_out.NormalInterp = normalize((model * vec4(normal, 0.0)).xyz);
//    
    gl_Position = projView * FragPos;
};

//
//
#shader[fragment]
#version 410 core
//
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

//
in VS_OUT
{
    vec2 TexCoord;
    vec3 FragPos;
    vec3 NormalInterp;
  //  vec3 Normal;
} vs_out;
//
uniform sampler2D texture_diffuse1;
////uniform sampler2D texture_specular1;
//
//
void main()
{
    // store the fragment position vector in the first gbuffer texture
    gPosition = vs_out.FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = vs_out.NormalInterp;
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(texture_diffuse1, vs_out.TexCoord).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
     //gAlbedoSpec.a = texture(texture_specular1, vs_out.TexCoord).r;
};