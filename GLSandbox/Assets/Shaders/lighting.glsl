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
    vs_out.ShadowCoord = (shadowSpaceMatrix * model) * vec4(vPos, 1.0);
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

float ShadowCalculation(vec4 ShadowPos)
{

    vec3 projCoords = ShadowPos.xyz / ShadowPos.w;
    float closestDepth = texture(depthTexture, projCoords.xy).r;
    float currentDepth = projCoords.z;
    //float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
    float bias = max(0.01 * (1.0 - dot(vs_out.Normal, lightDir)), 0.001);
    float shadow = 0.0;
    //shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    if (projCoords.z > 1.0)
        return 0;

    vec2 texelSize = 1.0 / textureSize(depthTexture, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depthTexture, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
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
        attenuation = 1;
    }



    lightDir = vec3(-Lights.Direction);

    float NdotL = max(dot(vs_out.NormalInterp, lightDir), 0.0);
    vec3 halfVec = normalize(lightDir + normalize(-vs_out.FragPos));
    float eyeLight = max(dot(vs_out.NormalInterp, halfVec), 0.0);
    float Specular;
    if (NdotL > 0.0)
        Specular = 1 * pow(eyeLight, 2);
    else
        Specular = 0;

    vec3 Ambient = vec3(Lights.Ambient) * Lights.AmbientEnergy * attenuation;
    vec3 Diffuse = vec3(Lights.Color) * NdotL * Lights.Energy * attenuation;

    float shadow = ShadowCalculation(vs_out.ShadowCoord);
    vec3 Light = Ambient + ((1.0 - shadow) * Diffuse);
    // Light = (Ambient) + (Diffuse);

  // vec3 color = min(Light * vec3(texCol), vec3(1.0));
   vec3 color = min(vec3(texCol), vec3(1.0));
    FragColor = vec4(color, texCol.a);

//    FragColor = vec4(vec3(texCol), 1);
};
//
//#shader[geometry]
//#version 430 core
//layout(triangles) in;
//layout(triangle, max_vertices = 6) out;
//
//in VS_OUT
//{
//    vec2 TexCoord;
//    vec3 FragPos;
//    vec3 NormalInterp;
//    vec4 ShadowCoord;
//    vec3 Normal;
//} gs_in[];
//
//out VS_OUT
//{
//    vec2 TexCoord;
//    vec3 FragPos;
//    vec3 NormalInterp;
//    vec4 ShadowCoord;
//    vec3 Normal;
//} vs_out;
//
//
//const float MAGNITUDE = 0.4;
//
//
//void GenerateLine(int index)
//{
//    //gl_Position = projection * gl_in[index].gl_Position;
//    //EmitVertex();
//    //gl_Position = projection * (gl_in[index].gl_Position +
//    //    vec4(gs_in[index].Normal, 0.0) * MAGNITUDE);
//    EmitVertex();
//    EndPrimitive();
//}
//
//void main()
//{
//    gl_Position = gl_in[].gl_Position;
//    EmitVertex();
//    EndPrimitive();
// //   GenerateLine(0); // first vertex normal
//   // GenerateLine(1); // second vertex normal
//   // GenerateLine(2); // third vertex normal
//}