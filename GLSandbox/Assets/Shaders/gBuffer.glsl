#shader [vertex]
#version 430 core


layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

#define MAX_SPLIT 4

out VS_OUT
{
    float FragDepth;
    vec2 TexCoord;
    vec3 FragPos;
    vec3 NormalInterp;
    vec3 Tangent;
    mat3 TBN;
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
    vs_out.FragDepth = abs(view * FragPos).z;
    vs_out.FragPos = (view * FragPos).rgb;
    vs_out.TexCoord = texCoord;
    vs_out.NormalInterp = normalize((view * model * vec4(normal, 0.0))).xyz; 
        vs_out.Tangent = tangent;
    vec3 bitan = bitangent;
    vec3 T = normalize((model * vec4(tangent, 0.0))).xyz;
    vec3 B = normalize((model * vec4(bitangent, 0.0))).xyz;
        vs_out.TBN = mat3(T, B, vs_out.NormalInterp);
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
    float FragDepth;
    vec2 TexCoord;
    vec3 FragPos;
    vec3 NormalInterp;
    vec3 Tangent;
    mat3 TBN;
} vs_out;


uniform sampler2D albedoTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

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


void main()
{
    vec4 albedo = texture(albedoTex, vs_out.TexCoord);
    gPosition = vec4(vs_out.FragPos, vs_out.FragDepth);
    vec3 Normal = vs_out.NormalInterp;
    if (Material.NormalEnabled == 1)
    {
        Normal = texture(normalTex, vs_out.TexCoord).xyz;

        Normal = Normal * 2.0 - 1.0;
        Normal = normalize(vs_out.TBN * Normal);

    }
    gNormal = Normal;
    gAlbedoSpec.rgb = albedo.rgb;
    gAlbedoSpec.a = texture(specularTex, vs_out.TexCoord).r;
 
};