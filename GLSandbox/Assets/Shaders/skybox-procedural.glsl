#shader vertex
#version 410 core

layout(location = 0) in vec3 vPos;
layout(location = 2) in vec2 uv;
layout(location = 0) out vec3 TexCoord; 

uniform mat4 projView;
uniform mat4 View;
uniform mat4 Proj;


const float Exposure = 1.3;
const vec3 GroundColor = vec3(0.3, 0.3, 0.3);
const vec3 SkyTint = vec3(1.0, 1.0, 1.0);
const float AtmosphereThickness = 1.0;


float lerp(float x, float y, float d)
{
    return x + (y - x) * d;
}

vec3 lerp(vec3 x, vec3 y, float d)
{
    return  x + (y - x) * d;
}
vec3 lerp(vec3 x, vec3 y, vec3 d)
{
    return x + (y - x) * d;
}


float getRayleighPhase(float eyeCos2)
{
    return 0.75 + 0.75*eyeCos2;
}

float getRayleighPhase(vec3 light, vec3 ray)
{
    float eyeCos = dot(light, ray);
    return getRayleighPhase(eyeCos);
}


#define powVec3(x, y) vec3(pow(x.x, y), pow(x.y, y), pow(x.z, y))
#define GAMMA 2.2
#define COLOR_2_GAMMA(color) powVec3(color, 1.0 / GAMMA)
#define COLOR_2_LINEAR(color) color
#define LINEAR_2_LINEAR(color) color

const vec3 kDefaultScatteringWavelength = vec3(.65, .57, .475);
const vec3 kVariableRangeForScatteringWavelength = vec3(.15, .15, .15);

#define OUTER_RADIUS 1.025
const float kOuterRadius = OUTER_RADIUS;
const float kOuterRadius2 = OUTER_RADIUS * OUTER_RADIUS;
const float kInnerRadius = 1.0;
const float kInnerRadius2 = 1.0;

const float kCameraHeight = 0.0001;

#define kRAYLEIGH lerp(0.0, 0.0025, pow(AtmosphereThickness, 2.5));
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
    gl_Position = vec4(vPos, 1.0);
    vs_out.pos = vec3(gl_Position);
    
    vec3 kSkyTintInGammaSpace = vec3(pow(SkyTint.x, 1.0 / GAMMA), pow(SkyTint.y, 1.0 / GAMMA), pow(SkyTint.z, 1.0 / GAMMA));
    vec3 kScatteringWavelength = lerp(kDefaultScatteringWavelength-kVariableRangeForScatteringWavelength, kDefaultScatteringWavelength+kVariableRangeForScatteringWavelength,  vec3(1.0,1.0,1.0) - kSkyTintInGammaSpace);


    vec3 kInvWavelength = 1.0 / vec3(pow(kScatteringWavelength.x, 4), pow(kScatteringWavelength.y, 4), pow(kScatteringWavelength.z, 4));
    float kKrESun = lerp(0.0, 0.0025, pow(AtmosphereThickness, 2.5)) * kSUN_BRIGHTNESS;
    float kKr4PI = lerp(0.0, 0.0025, pow(AtmosphereThickness, 2.5)) * 4.0 * 3.14159265;

    vec3 cameraPos = vec3(0, kInnerRadius + kCameraHeight, 0);
    vec3 eyeRay = transpose(mat3(View)) * (inverse(Proj) * gl_Position).xyz;

    float far = 0.0;
    vec3 cIn, cOut;

    if (eyeRay.y >= 0.0)
    {
        far = sqrt(kOuterRadius2 + kInnerRadius2 * eyeRay.y * eyeRay.y - kInnerRadius2) - kInnerRadius * eyeRay.y;
        vec3 pos = cameraPos + far * eyeRay;
        
        float height = kInnerRadius + kCameraHeight;
        float depth = exp(kScaleOverScaleDepth * (-kCameraHeight));
        float startAngle = dot(eyeRay, cameraPos) / height;
        float startOffset = depth * scale(startAngle);


        // Initialize the scattering loop variables
        float sampleLength = far / kSamples;
        float scaledLength = sampleLength * kScale;
        vec3 sampleRay = eyeRay * sampleLength;
        vec3 samplePoint = cameraPos + sampleRay * 0.5;

        // Now loop through the sample rays
        vec3 frontColor = vec3(0.0, 0.0, 0.0);

        {
            float height = length(samplePoint);
            float depth = exp(kScaleOverScaleDepth * (kInnerRadius - height));
            float lightAngle = dot(vec3(0, .3, .79), samplePoint) / height;
            float cameraAngle = dot(eyeRay, samplePoint) / height;
            float scatter = (startOffset + depth * (scale(lightAngle) - scale(cameraAngle)));
            vec3 attenuate = exp(-clamp(scatter, 0.0, kMAX_SCATTER) * (kInvWavelength * kKr4PI + kKm4PI));

            frontColor += attenuate * (depth * scaledLength);
            samplePoint += sampleRay;
        }
        {
            float height = length(samplePoint);
            float depth = exp(kScaleOverScaleDepth * (kInnerRadius - height));
            float lightAngle = dot(vec3(0, .3, .79), samplePoint) / height;
            float cameraAngle = dot(eyeRay, samplePoint) / height;
            float scatter = (startOffset + depth*(scale(lightAngle) - scale(cameraAngle)));
            vec3 attenuate = exp(-clamp(scatter, 0.0, kMAX_SCATTER) * (kInvWavelength * kKr4PI + kKm4PI));

            frontColor += attenuate * (depth * scaledLength);
            samplePoint += sampleRay;
        }

        // Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
        cIn = frontColor * (kInvWavelength * kKrESun);
        cOut = frontColor * kKmESun;
         //vs_out.sunColor = cOut;
    }

    else
    {
        // Ground
        far = (-kCameraHeight) / (min(-0.001, eyeRay.y));

        vec3 pos = cameraPos + far * eyeRay;

        // Calculate the ray's starting position, then calculate its scattering offset
        float depth = exp((-kCameraHeight) * (1.0/ kScaleDepth));
        float cameraAngle = dot(-eyeRay, pos);
        float lightAngle = dot(vec3(0, .3, .79), pos);
        float cameraScale = scale(cameraAngle);
        float lightScale = scale(lightAngle);
        float cameraOffset = depth*cameraScale;
        float temp = (lightScale + cameraScale);

        // Initialize the scattering loop variables
        float sampleLength = far / kSamples;
        float scaledLength = sampleLength * kScale;
        vec3 sampleRay = eyeRay * sampleLength;
        vec3 samplePoint = cameraPos + sampleRay * 0.5;

        // Now loop through the sample rays
        vec3 frontColor = vec3(0.0, 0.0, 0.0);
        vec3 attenuate;
//              for(int i=0; i<int(kSamples); i++) // Loop removed because we kept hitting SM2.0 temp variable limits. Doesn't affect the image too much.
        {
            float height = length(samplePoint);
            float depth = exp(kScaleOverScaleDepth * (kInnerRadius - height));
            float scatter = depth*temp - cameraOffset;
            attenuate = exp(-clamp(scatter, 0.0, kMAX_SCATTER) * (kInvWavelength * kKr4PI + kKm4PI));
            frontColor += attenuate * (depth * scaledLength);
            samplePoint += sampleRay;
        }

        cIn = frontColor * (kInvWavelength * kKrESun + kKmESun);
        cOut = clamp(attenuate, 0.0, 1.0);
    }


    vs_out.vPos          = -eyeRay;
    vs_out.groundColor = Exposure * (cIn + GroundColor * cOut);
    vs_out.skyColor    = Exposure * (cIn * getRayleighPhase(vec3(0, .3, .79), -eyeRay));
    float lightColorIntensity = clamp(length(vec3(1.0)), 0.25, 1.0);
    vs_out.sunColor    = kHDSundiskIntensityFactor * cOut * vec3(1.0) / lightColorIntensity;
  }



#shader fragment
#version 410 core

layout(location = 0) in vec3 TexCoord;


#define MIE_G -0.990
#define MIE_G2 0.9801
#define SKY_GROUND_THRESHOLD 0.02

const float SunSize = 0.04;
const float SunSizeConvergence = 5.0;

float getMiePhase(float eyeCos, float eyeCos2)
{
    float temp = 1.0 + MIE_G2 - 2.0 * MIE_G * eyeCos;
    temp = pow(temp, pow(SunSize,0.65) * 10);
    temp = max(temp,1.0e-4);
    temp = 1.5 * ((1.0 - MIE_G2) / (2.0 + MIE_G2)) * (1.0 + eyeCos2) / temp;
    return temp;
}

// Calculates the sun shape
float calcSunAttenuation(vec3 lightPos, vec3 ray)
{
    float focusedEyeCos = pow(clamp(dot(lightPos, ray), 0.0, 1.0), SunSizeConvergence);
    return getMiePhase(-focusedEyeCos, focusedEyeCos * focusedEyeCos);
}

in VS_OUT
{
    vec3 pos; 
    vec3 vPos;
    vec3 groundColor;
    vec3 skyColor;
    vec3 sunColor;
} vs_in;

out vec4 FragColor;


float lerp(float x, float y, float d)
{
    return x + (y - x) * d;
}
vec3 lerp(vec3 x, vec3 y, float d)
{
    return  x + (y - x) * d;
}
vec3 lerp(vec3 x, vec3 y, vec3 d)
{
    return x + (y - x) * d;
}


 void main()  
 {  

     //color = texture(cubeMap, TexCoord);
     vec3 col = vec3(0.0, 0.0, 0.0);

    // if y > 1 [eyeRay.y < -SKY_GROUND_THRESHOLD] - ground
    // if y >= 0 and < 1 [eyeRay.y <= 0 and > -SKY_GROUND_THRESHOLD] - horizon
    // if y < 0 [eyeRay.y > 0] - sky
    //#if SKYBOX_SUNDISK == SKYBOX_SUNDISK_HQ
     vec3 ray = normalize(vs_in.vPos);
     float y = ray.y / SKY_GROUND_THRESHOLD;
    //#elif SKYBOX_SUNDISK == SKYBOX_SUNDISK_SIMPLE
      //  vec3 ray = IN.rayDir.xyz;
        //float y = ray.y / SKY_GROUND_THRESHOLD;
    //#else
      //  float y = IN.skyGroundFactor;
    //#endif

        // if we did precalculate color in vprog: just do lerp between them
      col = lerp(vs_in.skyColor, vs_in.groundColor, clamp(y, 0.0, 1.0));

    //#if SKYBOX_SUNDISK != SKYBOX_SUNDISK_NONE
    if(y < 0.0)
    {
        col += vs_in.sunColor * calcSunAttenuation(vec3(0, .4, -1.0), -ray);
    }
    //#e//ndif

    //#if defined(UNITY_COLORSPACE_GAMMA) && !SKYBOX_COLOR_IN_TARGET_COLOR_SPACE
        //col = LINEAR_2_OUTPUT(col);
  //  #endif
   // FragColor = vec4(1.0);
   
	FragColor = vec4(-vs_in.sunColor, 1.0);
 };