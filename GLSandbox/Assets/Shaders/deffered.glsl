#shader vertex
#version 430 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

#define MAX_SPLIT 4


void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}

#shader fragment
#version 430 core

#define USE_PCF

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

//*** [Lighting] ***//

struct LightProperties
{
    int LightType;
    float AmbientEnergy;
    float Energy; 
    vec3 Ambient; 
    vec3 Color;
    vec3 Direction;
    //vec3 Position;
};


uniform vec3 ViewPosition;

layout(std140, binding = 0) uniform Matrices
{
   mat4 view;
   mat4 proj;
   mat4 model;
   mat4 shadowSpaceMatrix[4];
};


layout(std140, binding = 1) uniform LightsUniform
{
    LightProperties Lights;
};

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

void main()
{             
    vec4 FragPos = texture(gPosition, TexCoords);
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;

    vec3 Color = Albedo; 
    vec3 ViewDir = normalize(ViewPosition - FragPos.rgb);

    vec3 LightDir = -Lights.Direction;

    float NdotL = max(dot(Normal, LightDir), 0.0);
    vec3 halfVec = normalize(LightDir + normalize(FragPos.rgb));
    float eyeLight = max(dot(Normal, halfVec), 0.0);
    float Specular = 0;

    if (NdotL > 0.0)
        Specular = pow(eyeLight, 2);

    vec3 Ambient = (Lights.Ambient * Lights.AmbientEnergy) * 1.0;
    vec3 Diffuse = Lights.Color * NdotL * Lights.Energy * 1.0;
    vec3 Light = Ambient + Diffuse;

 
    Color = min(Light * Color, vec3(1));

//    const float gamma = 2.2;
  //  vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
  
    // reinhard tone mapping
    //vec3 mapped = Color / (Color + vec3(1.0));
    // gamma correction 
    //mapped = pow(mapped, vec3(1.0 / gamma));

    // Yxy.x is Y, the luminance
    vec3 Yxy = convertRGB2Yxy(Color);


    Yxy.x *= .6;

    Color = convertYxy2RGB(Yxy);

    Color.x = Tonemap_ACES(Color.x);
    Color.y = Tonemap_ACES(Color.y);
    Color.z = Tonemap_ACES(Color.z);

  //  Color = pow(Color, vec3(1.0 / 2.2));
//  gl_FragColor = toGamma(vec4(rgb, 1.0) );
    FragColor = vec4(Color, 1.0);
}  