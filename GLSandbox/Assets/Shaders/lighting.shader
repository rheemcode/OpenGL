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
    float FragDist;
    vec2 TexCoord;
    vec3 FragPos;
    vec3 NormalInterp;
    vec3 Tangent;
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

    vs_out.Tangent = tangent;
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


#shader[fragment]
#version 430 core


#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2
#define POINT_LIGHT 3


in VS_OUT
{
    float FragDist;
    vec2 TexCoord;
    vec3 FragPos;
    vec3 NormalInterp;
    vec3 Tangent;
    vec4 ShadowCoord[4];
    mat3 TBN;
} vs_out;


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

struct MaterialProperties
{
    int NormalEnabled;
    float Shininess;
    float SpecularHighlight;
    vec3 Diffuse;
    vec3 Ambient;
};

uniform float farDistance[4];
uniform sampler2DArray depthTexture;

uniform sampler2D albedoTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

layout(std140, binding = 1) uniform LightsUniform
{
    LightProperties Lights;
};


layout(std140, binding = 2) uniform MaterialUniform
{
    MaterialProperties Material;
};


out vec4 FragColor;


vec3 lightDir;
float attenuation;

const int pcfCount = 2;
const int samples = 8;
const float totalTexels = (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);
const float mapSize = 2048.0 ;
const float texelSize = 1.0 / mapSize;

const int nsamples = 8;
uniform vec2 offsets[nsamples] = { vec2(0.000000, 0.000000),
                                  vec2(0.079821, 0.165750),
                                  vec2(-0.331500, 0.159642),
                                  vec2(-0.239463, -0.49725),
                                  vec2(0.662999, -0.319284),
                                  vec2(0.399104, 0.828749),
                                  vec2(-0.994499, 0.478925),
                                  vec2(-0.558746, -1.160249) };

vec3 convertRGB2XYZ(vec3 _rgb)
{
    // Reference:
    // RGB/XYZ Matrices
    // http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
    vec3 xyz;
    xyz.x = dot(vec3(0.4124564, 0.3575761, 0.1804375), _rgb);
    xyz.y = dot(vec3(0.2126729, 0.7151522, 0.0721750), _rgb);
    xyz.z = dot(vec3(0.0193339, 0.1191920, 0.9503041), _rgb);
    return xyz;
}

vec3 convertXYZ2RGB(vec3 _xyz)
{
    vec3 rgb;
    rgb.x = dot(vec3(3.2404542, -1.5371385, -0.4985314), _xyz);
    rgb.y = dot(vec3(-0.9692660, 1.8760108, 0.0415560), _xyz);
    rgb.z = dot(vec3(0.0556434, -0.2040259, 1.0572252), _xyz);
    return rgb;
}

vec3 convertXYZ2Yxy(vec3 _xyz)
{
    // Reference:
    // http://www.brucelindbloom.com/index.html?Eqn_XYZ_to_xyY.html
    float inv = 1.0 / dot(_xyz, vec3(1.0, 1.0, 1.0));
    return vec3(_xyz.y, _xyz.x * inv, _xyz.y * inv);
}

vec3 convertYxy2XYZ(vec3 _Yxy)
{
    // Reference:
    // http://www.brucelindbloom.com/index.html?Eqn_xyY_to_XYZ.html
    vec3 xyz;
    xyz.x = _Yxy.x * _Yxy.y / _Yxy.z;
    xyz.y = _Yxy.x;
    xyz.z = _Yxy.x * (1.0 - _Yxy.y - _Yxy.z) / _Yxy.z;
    return xyz;
}

vec3 convertRGB2Yxy(vec3 _rgb)
{
    return convertXYZ2Yxy(convertRGB2XYZ(_rgb));
}

vec3 convertYxy2RGB(vec3 _Yxy)
{
    return convertXYZ2RGB(convertYxy2XYZ(_Yxy));
}

float Tonemap_ACES(float x)
{
    // Narkowicz 2015, "ACES Filmic Tone Mapping Curve"
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return (x * (a * x + b)) / (x * (c * x + d) + e);
}

int getDepth()
{
    float fDistance = vs_out.FragDist;
    int index = 3;

    if (fDistance < farDistance[0])
        index = 0;
    else if (fDistance < farDistance[1])
        index = 1;
    else if (fDistance < farDistance[2])
        index = 2;
   
    return index;
}

float SampleShadowMap(vec2 coords, float compare, int index)
{
    return step(compare, texture(depthTexture, vec3(coords.xy, index)).r);
}

float SampleShadowMapLinear(vec2 coords, float compare, int index)
{
    vec2 pixelPos = coords / texelSize + vec2(0.5);
    vec2 fractPart = fract(pixelPos);
    vec2 startTexel = (pixelPos - fractPart) * texelSize;

    float blTexel = SampleShadowMap(startTexel, compare, index);
    float brTexel = SampleShadowMap(startTexel + vec2(texelSize, 0.0), compare, index);
    float tlTexel = SampleShadowMap(startTexel + vec2(0.0, texelSize), compare, index);
    float trTexel = SampleShadowMap(startTexel + texelSize, compare, index);

    float mixA = mix(blTexel, tlTexel, fractPart.y);
    float mixB = mix(brTexel, trTexel, fractPart.y);

    return mix(mixA, mixB, fractPart.x);
}

float SampleShadowMapPCF(vec4 ShadowPos, int index)
{

    vec3 projCoords = ShadowPos.xyz / ShadowPos.w;

    if (projCoords.z >= 1.0 || projCoords.z < 0.0)
        return 0.0;

    float currentDepth = projCoords.z;
    float bias = max(0.001 * (1.0 - dot(vs_out.NormalInterp, lightDir)), 0.001);
    float shadow = 0.0;
    vec2 grad = fract(projCoords.xy * 2048 + 0.5f);

    vec2 offset;
    for (int x = -pcfCount; x <= pcfCount; ++x)
    {
        for (int y = -pcfCount; y <= pcfCount; ++y)
        {
            offset = vec2(x, y) * texelSize;
            vec4 tmp = textureGather(depthTexture, vec3(projCoords.xy + offset, index));
            tmp.x = tmp.x < projCoords.z - 0.001 ? 0.0f : 1.0f;
            tmp.y = tmp.y < projCoords.z - 0.001 ? 0.0f : 1.0f;
            tmp.z = tmp.z < projCoords.z - 0.001 ? 0.0f : 1.0f;
            tmp.w = tmp.w < projCoords.z - 0.001 ? 0.0f : 1.0f;
            shadow += mix(mix(tmp.w, tmp.z, grad.x), mix(tmp.x, tmp.y, grad.x), grad.y);
            //float pcfDepth = SampleShadowMapLinear(projCoords.xy + offset, currentDepth - 0.001, index);

            //shadow += currentDepth - 0.01 > pcfDepth ? 1.0 : 0.0;
            //shadow += pcfDepth;

        }
    }

    //for (int i = 0; i < nsamples; i++)
    //{
    //    vec2 shadow_lookup = projCoords.xy + texelSize * offsets[i]; //scale the offsets to the texture size, and make them twice as large to cover a larger radius
    //    shadow += currentDepth - 0.001 > texture(depthTexture, vec3(shadow_lookup, index)).x ? 1.0 : 0.0;
    //}

    shadow /= totalTexels;
    return shadow;
}



void main()
{
    vec4 AlbedoCol = texture(albedoTexture, vs_out.TexCoord);
    if (AlbedoCol.a == 0)
        discard;
    vec4 SpecularCol = texture(specularTexture, vs_out.TexCoord).xxxx;
    vec3 Normal = vs_out.NormalInterp;
    if (Material.NormalEnabled == 1)
    {
        Normal = texture(normalTexture, vs_out.TexCoord).xyz;

        Normal = Normal * 2.0 - 1.0;
        Normal = normalize(vs_out.TBN * Normal);

    }
    
    float attenuation = 1.0;

    lightDir = vec3(-Lights.Direction);

    float NdotL = max(dot(Normal, lightDir),0.0);
    vec3 halfVec = normalize(lightDir + normalize(-vs_out.FragPos));
    float eyeLight = max(dot(Normal, halfVec), 0.0);
    float Spec = 0;
  
    if (NdotL > 0.0)
        Spec = 1.0 * pow(eyeLight, Material.Shininess * 0.5);

    vec3 Ambient = vec3(Lights.Ambient) * Lights.AmbientEnergy;
    vec3 Diffuse = vec3(Lights.Color) * NdotL * (Lights.Energy * 1.3);
    vec3 Specular = Lights.Color * Spec * SpecularCol * Material.SpecularHighlight;

    int shadowIndex = getDepth();
    float shadow = SampleShadowMapPCF(vs_out.ShadowCoord[shadowIndex], shadowIndex);
    vec3 Light = (Ambient * 0.3231) +   (((shadow * vs_out.ShadowCoord[shadowIndex].w)) * Diffuse);
    vec3 color = min(Light * vec3(AlbedoCol), vec3(1));

    vec3 Yxy = convertRGB2Yxy(color);


    Yxy.x *= .6;

    color = convertYxy2RGB(Yxy);

    color.x = Tonemap_ACES(color.x);
    color.y = Tonemap_ACES(color.y);
    color.z = Tonemap_ACES(color.z);

    color = pow(color, vec3(1.0 / 2.2));
  //  gl_FragColor = toGamma(vec4(rgb, 1.0) );
    //FragColor = vec4(Color, 1.0);
    FragColor = vec4(color, 1.0);
    

};