#shader vertex
#version 430 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}

#shader fragment
#version 430 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;


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

uniform vec3 ViewPosition;

layout(std140, binding=1) uniform LightsUniform
{
    LightProperties Lights;
};



void main()
{             
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;

    vec3 Color = Albedo * Lights.Ambient; // hard coded ambeint component;
    vec3 ViewDir = normalize(ViewPosition - FragPos);

    vec3 LightDir = -Lights.Direction;
    vec3 Diffuse = max(dot(Normal, LightDir), 0.0) * Albedo * Lights.Color;
    Color += Diffuse;
    FragColor = vec4(Normal, 1.0);
}  