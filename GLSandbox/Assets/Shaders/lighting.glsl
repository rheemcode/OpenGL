//#shader [vertex]
#version 430 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

#define MAX_SPLIT 4
    
out VS_OUT
{
    float FragDist;
    vec2 TexCoord;
    vec3 FragPos;
    vec3 NormalInterp;
    vec4 ShadowCoord[MAX_SPLIT];
    mat3 TBN;

} vs_out;

layout(std140, binding = 0) uniform Matrices
{
   mat4 view;
   mat4 proj; // offset 0
   mat4 model; // offset 16 * 4
   mat4 shadowSpaceMatrix[MAX_SPLIT]; //  128
};

uniform mat4 bias = mat4(vec4(0.5, 0.0, 0.0, 0),
                        vec4(0.0, 0.5, 0.0, 0),
                        vec4(0.0, 0.0, 0.5, 0),
                        vec4(0.5, 0.5, 0.5, 1.0));
void main()
{
    vec4 FragPos = model * vec4(vPos, 1.0);
    vs_out.FragPos = FragPos.xyz;
    vs_out.FragDist = abs(view * FragPos).z;

    for (int i = 0; i < MAX_SPLIT; i++)
        vs_out.ShadowCoord[i] = bias * shadowSpaceMatrix[i] * FragPos;

    //vs_out.Normal = (view * transpose(inverse(mat3(model))) * normal).xyz;
    vs_out.TexCoord = texCoord;
    vs_out.NormalInterp = normalize((model * vec4(normal, 0.0))).xyz;

    vec3 tang = tangent;
    vec3 bitan = bitangent;
    vec3 T = normalize((model * vec4(tangent, 0.0))).xyz;
    vec3 B = normalize((model * vec4(bitangent, 0.0))).xyz;


    //vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    //vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    //// re-orthogonalize T with respect to N
    //T = normalize(T - dot(T, N) * N);
    //// then retrieve perpendicular vector B with the cross product of T and N
    //vec3 B = cross(N, T);

    //mat3 TBN = mat3(T, B, N)

    vs_out.TBN = mat3(T, B, vs_out.NormalInterp);
    gl_Position = proj * view * FragPos;
};
//
//
//#shader[fragment]
//#version 430 core
//
//
//#define DIRECTIONAL_LIGHT 1
//#define SPOT_LIGHT 2
//#define POINT_LIGHT 3
//
//
//in VS_OUT
//{
//    float FragDist;
//    vec2 TexCoord;
//    vec3 FragPos;
//    vec3 NormalInterp;
//    vec4 ShadowCoord[4];
//    mat3 TBN;
//} vs_out;
//
//
////*** [Lighting] ***//
//
//struct LightProperties
//{
//    int LightType; // 0
//    float AmbientEnergy; // 4
//    float Energy; // 8
//    vec3 Ambient; // 16
//    vec3 Color; // 32
//    vec3 Direction; // 48
//    //vec3 Position;
//};
//
//struct MaterialProperties
//{
//    int NormalEnabled;
//    float Shininess;
//    float SpecularHighlight;
//    vec3 Diffuse;
//    vec3 Ambient;
//};
//
//uniform float farDistance[4];
//uniform sampler2DArray depthTexture;
//
//uniform sampler2D albedoTexture;
//uniform sampler2D specularTexture;
//uniform sampler2D normalTexture;
//
//layout(std140, binding = 1) uniform LightsUniform
//{
//    LightProperties Lights;
//};
//
//
//layout(std140, binding = 2) uniform MaterialUniform
//{
//    MaterialProperties Material;
//};
//
//
//out vec4 FragColor;
//
//
//vec3 lightDir;
//float attenuation;
//
//const int pcfCount = 2;
//const int samples = 4;
//const float totalTexels = (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);
//const float mapSize = 2048.0 ;
//const float texelSize = 1.0 / mapSize;
//
//int getDepth()
//{
//    float fDistance = vs_out.FragDist;
//    ////return 0;
//    ////int count = 0;
//    //for (int i = 0; i < 4;  i++)
//    //{
//
//    //    if (fDistance < farDistance[i])
//    //    {
//    //        return i;
//    //    }
//
//    //    continue;
//    //}
//
//    return 0;
//    //return 0;
//    int index = 3;
//
//    if (fDistance < farDistance[0])
//        index = 0;
//    else if (fDistance < farDistance[1])
//        index = 1;
//    else if (fDistance < farDistance[2])
//        index = 2;
//   
//    return index;
//}
//
//float ShadowCalculation(vec4 ShadowPos, int index)
//{
//
//    vec3 projCoords = ShadowPos.xyz / ShadowPos.w;
//
//    if (projCoords.z >= 1.0 || projCoords.z < 0.0)
//        return 0.0;
//
//    float currentDepth = projCoords.z;
//    float bias = max(0.001 * (1.0 - dot(vs_out.NormalInterp, lightDir)), 0.001);
//    float shadow = 0.0;
//
//    vec2 offset;
//    for (int x = -pcfCount; x <= pcfCount; ++x)
//    {
//        for (int y = -pcfCount; y <= pcfCount; ++y)
//        {
//            offset.x = x; offset.y = y;
//            float pcfDepth = texture(depthTexture, vec3(projCoords.xy + offset * texelSize, index)).r;
//            shadow += currentDepth - 0.001 > pcfDepth ? 1.0 : 0.0;
//        }
//    }
//
//    shadow /= totalTexels;
//    return shadow * 0.6f;
//}
//
//
//
//void main()
//{
//    vec4 AlbedoCol = texture(albedoTexture, vs_out.TexCoord);
//    vec4 SpecularCol = texture(specularTexture, vs_out.TexCoord);
//    
//    vec3 Normal;
//   if (Material.NormalEnabled == 1)
//        Normal = texture(normalTexture, vs_out.TexCoord).xyz;
//
//    Normal = Normal * 2.0 - 1.0;
//    Normal = normalize(vs_out.TBN * Normal);
//
//    float attenuation = 1.0;
//
//    lightDir = vec3(-Lights.Direction);
//
//    float NdotL = max(dot(vs_out.NormalInterp, lightDir),0.0);
//    vec3 halfVec = normalize(lightDir + normalize(-vs_out.FragPos));
//    float eyeLight = max(dot(vs_out.NormalInterp, halfVec), 0.0);
//    float Spec = 0;
//  
//    if (NdotL > 0.0)
//        Spec = 1.0 * pow(eyeLight, Material.Shininess);
//
//    vec3 Ambient = vec3(Lights.Ambient) * Lights.AmbientEnergy;
//    vec3 Diffuse = vec3(Lights.Color) * NdotL *  Lights.Energy;
//    vec3 Specular = Lights.Color * Spec * SpecularCol;
//
//    int shadowIndex = getDepth();
//    float shadow = ShadowCalculation(vs_out.ShadowCoord[shadowIndex], shadowIndex);
//    vec3 Light = Ambient + Specular + ((1.0 - (shadow * vs_out.ShadowCoord[shadowIndex].w)) * Diffuse);
//    vec3 color = min(Light * vec3(AlbedoCol), vec3(1));
//
//    FragColor = vec4(color, 1.0);
//
//};