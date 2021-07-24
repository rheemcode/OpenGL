#shader vertex
#version 410 core


layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec2 TexCoord;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec3 FragPos;
layout(location = 3) out vec3 VertexInterp;
layout(location = 4) out vec3 NormalInterp;

uniform mat4 view;
uniform mat4 proj;
uniform mat4 projView;
uniform mat4 model;

void main()
{
    gl_Position = projView * model * vec4(vPos, 1.0);
    TexCoord = texCoord;
 //   Normal = mat3(transpose(inverse(model))) * normal;
    NormalInterp = normalize((model * vec4(normal, 0.0)).xyz);
 //   VertexInterp = (model * vec4(vPos, 1.0)).xyz;
    FragPos = vec3(model * vec4(vPos, 1.0));
};



#shader fragment
#version 410 core

layout(location = 0) in vec2 TexCoord;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 FragPos;
layout(location = 4) in vec3 NormalInterp;

//*** [Lighting] ***//
uniform vec4  ViewPosition;
uniform float SpecularStrength;
uniform float Shininess;
uniform float AmbientEnergy;

struct LightProperties
{
    int LightType;
    vec3 Ambient;
    vec3 Color;
    vec3 Position;
    vec3 Direction;
    float Energy;
    float Radius;
    vec2 LightAttenuation;
    float Cutoff;
    float OuterCutoff;
    bool Use;
};

struct MaterialProperties
{   
    sampler2D diffuseMap;
    vec4 Color;
    float Shininess;
    float SpecularHighlights;
};

uniform LightProperties Lights[100];
uniform MaterialProperties Material;

out vec4 FragColor;


vec3 lightDir;
float attenuation;

void main()
{
    vec3 ScatteredLight = vec3(0);
    vec3 ReflectedLight = vec3(0);

    for (int light = 0; light < 100; ++light)
    {
        if (!Lights[light].Use)
            continue;

        float attenuation;
        vec3 normal = NormalInterp;

        if (Lights[light].LightType == 1) // Directional Light
        {
            lightDir = -Lights[light].Direction;
            attenuation = 1;
        }
        else if (Lights[light].LightType == 2) // Point Light
        {
            lightDir = Lights[light].Position - FragPos;
            float radius = Lights[light].Radius;
            float dist = min(length(lightDir), radius);
            lightDir = normalize(lightDir);
            attenuation = pow(max(1.0 - dist / radius, 0.0), Lights[light].LightAttenuation.x) * Lights[light].LightAttenuation.y;
            // attenuation = radius / (pow(dist, 2.0) + 1.0);
        }

        else if (Lights[light].LightType == 3) // Spot Light
        {
            lightDir = Lights[light].Position - FragPos;
            float radius = Lights[light].Radius;
            float dist = min(length(lightDir), radius);
            lightDir = normalize(lightDir);
            vec3 spotLightDir = Lights[light].Direction;
            float lightCutoff = Lights[light].Cutoff;
            float lightOuterCutoff = Lights[light].OuterCutoff;

            float scos = dot(lightDir, -spotLightDir);
            float epsilon = lightCutoff - lightOuterCutoff;
            float intensity = clamp((scos - lightOuterCutoff) / epsilon, 0.0, 1.0);

            attenuation = 1.0 / (1 + Lights[light].LightAttenuation.x * dist + Lights[light].LightAttenuation.y * (dist * dist));
            attenuation *= intensity;
        }

        float NdotL = max(dot(normal, lightDir), 0.0);
        vec3 halfVec = normalize(lightDir + normalize(-FragPos));
        float eyeLight = max(dot(normal, halfVec), 0.0);

        float Specular;
        if (NdotL > 0.0)
            Specular =  pow(eyeLight, 1);
        else
            Specular = 0;

        vec3 Ambient = Lights[light].Ambient * AmbientEnergy * attenuation;
        vec3 Diffuse = Lights[light].Color * Lights[light].Energy * NdotL * attenuation;

        ScatteredLight += Ambient + Diffuse;
        ReflectedLight += Specular * attenuation;

        vec3 Light = (Ambient + Diffuse + Specular) * attenuation;

    }
    vec3 color;

    color = min((ScatteredLight + ReflectedLight) * vec3(texture(Material.diffuseMap, TexCoord)), vec3(1.0));
    FragColor = vec4(color, Material.Color.a);

};