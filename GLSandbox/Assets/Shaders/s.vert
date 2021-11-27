#version 410 core

layout(location = 0) in vec3 vPos;
layout(location = 0) out vec3 TexCoord; 

uniform mat4 projView;


uniform float Exposure;
uniform vec3 GroundColor;
uniform float SunSize;
uniform float SunSizeConvergence;
uniform vec3 SkyTint;
uniform float AtmosphereThickness;


float lerp(float x, float y, float d)
{
    return x + (y - x) * d;
}
vec3 lerp(vec3 x, vec3 y, float d)
{
    return vec3(lerp(x.x, y.x, d), lerp(x.y, y.y, d), lerp(x.z, y.z, d));
}
vec3 lerp(vec3 x, vec3 y, vec3 d)
{
    return vec3(lerp(x.x, y.x, d.x), lerp(x.y, y.y, d.y), lerp(x.z, y.z, d.z));
}


#define powVec3(x, y) vec3(pow(x.x, y), pow(x.y, y), pow(x.z, y))
#define GAMMA 2.2
#define COLOR_2_GAMMA(color) powVec3(color, 1.0/GAMMA)
#define COLOR_2_LINEAR(color) color
#define LINEAR_2_LINEAR(color) color

#define OUTER_RADIUS 1.025
const float kOuterRadius = OUTER_RADIUS;
const float kOuterRadius2 = OUTER_RADIUS * OUTER_RADIUS;
const float kOuterInner = 1.0;
const float kOuterInner2 = 1.0;

const vec3 kDefaultScatteringWavelength = vec3(.65, .57, .475);
const vec3 kVariableRangeForScatteringWavelength = vec3(.15, .15, .15);

const float kCameraHeight = 0.0001;

#define kRAYLEIGH (lerp(0.0, 0.0025, pow(AtmosphereThickness, 2.5)));
#define kMIE 0.0010
#define kSUN_BRIGHTNESS 20.0
#define kMAX_SCATTER 50.0

const float kHDSundiskIntensityFactor = 15.0;
const float kSimpleSundiskIntensityFactor = 27.0;
const float kSunScale = 400.0 * kSUN_BRIGHTNESS;
const float kKmESun = kMIE * kSUN_BRIGHTNESS;
const float kKm4PI = kMIE * 4.0 * 3.14159265;
const float kScale = 1.0 / (OUTER_RADIUS - 1.0);
const float kScaleDepth = 0.25;
const float kScaleOverScaleDepth = (1.0 / (OUTER_RADIUS - 1.0)) / 0.25;
const float kSamples = 2.0; // THIS IS UNROLLED MANUALLY, DON'T TOUCH

#define MIE_G (-0.990)
#define MIE_G2 0.9801
#define SKY_GROUND_THRESHOLD 0.02


#define SKYBOX_SUNDISK_NONE 0
// simplistic sun disk - without mie phase function
#define SKYBOX_SUNDISK_SIMPLE 1
// full calculation - uses mie phase function
#define SKYBOX_SUNDISK_HQ 2

#ifndef SKYBOX_SUNDISK
    #if defined(_SUNDISK_NONE)
        #define SKYBOX_SUNDISK SKYBOX_SUNDISK_NONE
    #elif defined(_SUNDISK_SIMPLE)
        #define SKYBOX_SUNDISK SKYBOX_SUNDISK_SIMPLE
    #else
        #define SKYBOX_SUNDISK SKYBOX_SUNDISK_HQ
    #endif
#endif

#ifndef SKYBOX_COLOR_IN_TARGET_COLOR_SPACE
    #if defined(SHADER_API_MOBILE)
        #define SKYBOX_COLOR_IN_TARGET_COLOR_SPACE 1
    #else
        #define SKYBOX_COLOR_IN_TARGET_COLOR_SPACE 0
    #endif
#endif

float scale(float inCos)
{
    float x = 1.0 - inCos;
    return 0.25 * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}


out VS_OUT
{
    vec3 pos; 
    vec3 vPos;
    vec3 groundColor;
    vec3 skyColor;
    vec3 sunColor;
} vs_out;

#define GAMMAINV 1.0 / GAMMA



void main()  
{  
    gl_Position = projView * vec4(vPos, 1.0);
    vs_out.pos = vec3(gl_Position);
    
    
   vec3 kSkyTintInGammaSpace = vec3(pow(SkyTint.x, GAMMAINV), pow(SkyTint.y, GAMMAINV), pow(SkyTint.z, GAMMAINV));
    vec3 kScatteringWavelength = lerp(kDefaultScatteringWavelength-kVariableRangeForScatteringWavelength,
                 kDefaultScatteringWavelength+kVariableRangeForScatteringWavelength,
                 vec3(1.0,1.0,1.0) - kSkyTintInGammaSpace);
    TexCoord = vPos;
}



#shader fragment
#version 410 core

layout(location = 0) in vec3 TexCoord;




float getRayleighPhase(float eyeCos2)
{
    return 0.75 + 0.75*eyeCos2;
}

float getRayleighPhase(vec3 light, vec3 ray)
{
    float eyeCos = dot(light, ray);
    return getRayleighPhase(eyeCos);
}


out vec4 FragColor;

 void main()  
 {  
     //color = texture(cubeMap, TexCoord);
    vec3 colour;

	int cubeMapOnly = 0;
	if(cubeMapOnly > 0.0)
	{
		colour = textureLod(cubeMap, TexCoord.xyz, blurLevel).xyz;
	}
	else
	{
		colour = getSky();
	}

	//outFrag = vec4(colour, 1.0);
  //  color = vec4(brightnessContrast(color.xyz, 1.15f, 1.15f), color.w);
  //  color = vec4(gamma(color.xyz, 4.8f),color.w);
	FragColor = vec4(colour, 1.0);
 };