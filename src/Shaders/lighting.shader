#shader vertex
#version 410 core


layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec2 TexCoord;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec3 FragPos;

uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;

void main()
{
    gl_Position = proj * view * model * vec4(vPos, 1.0);
    TexCoord = texCoord;
    Normal = mat3(transpose(inverse(model))) * normal;
    FragPos = vec3(model * vec4(vPos, 1.0));
};



#shader fragment
#version 410 core

layout(location = 0) in vec2 TexCoord;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 FragPos;


struct PointLight
{
    vec4 LightColor;
    vec3 Position;

    vec2 LightAttenuation;
    float Radius;
    float Energy;
    bool Use;
};

struct SpotLight
{
    vec4 LightColor;
    vec3 Position;
    vec3 Direction;

    vec2 LightAttenuation;
    float Energy;
    float Radius;

    float InnerCutoff;
    float OuterCutoff;
    bool use;
};

struct DirectionalLight
{
    vec3 Direction;
    vec4 LightColor;

    float Energy;
    bool use;
};

//*** [Lighting] ***//
uniform vec4 AmbientColor;
uniform float AmbientStrength;
uniform vec4 ViewPosition;
uniform float SpecularStrength;
uniform float Shininess;


uniform U_DirectionalLight
{
    vec3 Direction;
    vec4 LightColor;

    float Energy;
    bool use;
} directionalLight;

uniform U_PointLight
{
    vec4 LightColor;
    vec3 Position;

    vec2 LightAttenuation;
    float Radius;
    float Energy;
    bool use;

} pointLight;

uniform U_SpotLight
{
    vec4 LightColor;
    vec3 Position;
    vec3 Direction;

    vec2 LightAttenuation;
    float Energy;
    float Radius;

    float InnerCutoff;
    float OuterCutoff;
    bool use;
} spotLight;

out vec4 FragColor;

vec4 CalculatePointLight(PointLight p_PointLight)
{
    vec3 normal = normalize(Normal);
    float lightRadius = p_PointLight.Radius;

    vec3 lightDir = p_PointLight.Position - vec3(FragPos);
    float lightDistance = min(length(lightDir), lightRadius);
    lightDir = normalize(lightDir);
    float attenuation = pow(max(1.0 - lightDistance / lightRadius, 0.0), p_PointLight.LightAttenuation.x) * p_PointLight.LightAttenuation.y;

    vec3 viewDir = normalize(vec3(ViewPosition) - vec3(FragPos));


    float diffuse = max(dot(normal, lightDir), 0.0);
    vec3 halfVec = normalize(lightDir + vec3(ViewPosition));
    float eyeLight = max(dot(normal, halfVec), 0.0);

    vec4 Diffuse = p_PointLight.LightColor * diffuse * attenuation;
    vec4 Ambient = AmbientStrength * AmbientColor;
    vec4 Specular;

    if (diffuse > 0.0)
    {
        Specular = p_PointLight.LightColor * SpecularStrength * pow(eyeLight, Shininess) * attenuation;
    }

    else
    {
        Specular = vec4(0);
    }

    return Ambient + Diffuse + Specular * vec4(1, 0, 0, 1.0);

}

vec4 CalculateDirectionalLight(DirectionalLight p_DirectionalLight)
{
    vec3 normal = normalize(Normal);
    vec3 lightDir = -p_DirectionalLight.Direction;
    vec3 viewDir = normalize(vec3(ViewPosition) - vec3(FragPos));


    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 halfVec = normalize(lightDir + normalize(vec3(ViewPosition)));

    float eyeLight = max(dot(normal, halfVec), 0.0);

    vec4 Diffuse = p_DirectionalLight.LightColor * NdotL;
    vec4 Ambient = AmbientStrength * AmbientColor;
    vec4 Specular;
    vec4 v;
    if (NdotL > 0.0)
    {
        Specular = SpecularStrength * pow(eyeLight, Shininess) * p_DirectionalLight.LightColor;
    }

    else
    {
        Specular = vec4(0);
    }

    return (Ambient + Diffuse + Specular) * vec4(.7, .7, .7, 1.0);

}

//
//
//vec4 CalculateDirectionalLight(DirectionalLight p_DirectionalLight)
//{
//    vec3 normal = normalize(Normal);
//    vec3 lightDir = normalize(-p_DirectionalLight.Direction);
//    vec3 viewDir = normalize(vec3(ViewPosition) - vec3(FragPos));
//
//    vec3 reflectDir = reflect(lightDir, normal);
//
//    float diffuse = max(dot(normal, lightDir), 0.0);
//    float eyeLight = max(dot(normal, -reflectDir), 0.0);
//
//    vec4 Diffuse = p_DirectionalLight.LightColor * diffuse;
//    vec4 Ambient = AmbientStrength * AmbientColor;
//    vec4 Specular;
//  
//    //if (diffuse > 0)
//        Specular = SpecularStrength * pow(eyeLight, Shininess) * p_DirectionalLight.LightColor;
//   // else
//     //   Specular = vec4(0);
//
//
//    return  (Ambient + Diffuse + Specular) * vec4(1, 0, 0, 1.0);
//
//}


vec4 CalculateSpotLight(SpotLight p_SpotLight)
{
    vec3 normal = normalize(Normal);
    float lightRadius = p_SpotLight.Radius;

    vec3 lightDir = p_SpotLight.Position - vec3(FragPos);
    float lightDistance = min(length(lightDir), lightRadius);
    lightDir = normalize(lightDir);
    float attenuation = pow(max(1.0 - lightDistance / lightRadius, 0.0), p_SpotLight.LightAttenuation.x) * p_SpotLight.LightAttenuation.y;


    vec3 spotLightDir = normalize(p_SpotLight.Direction);

    float lightCutoff = p_SpotLight.InnerCutoff;
    float lightOuterCutoff = p_SpotLight.OuterCutoff;

    float scos = max(dot(lightDir, -spotLightDir), lightOuterCutoff);

    float epsilon = lightCutoff - lightOuterCutoff;
    float intensity = clamp((scos - lightOuterCutoff) / epsilon, 0.0, 1.0);
    attenuation *= intensity;
    vec3 viewDir = normalize(vec3(ViewPosition) - vec3(FragPos));


    float diffuse = max(dot(normal, lightDir), 0.0);
    vec3 halfVec = normalize(lightDir + vec3(ViewPosition));

    float eyeLight = max(dot(normal, halfVec), 0.0);

    vec4 Diffuse = p_SpotLight.LightColor * diffuse * attenuation;
    vec4 Ambient = AmbientStrength * AmbientColor;
    vec4 Specular;

    if (diffuse > 0.0)
    {
        Specular = p_SpotLight.LightColor * SpecularStrength * pow(eyeLight, Shininess) * attenuation;
    }

    else
    {
        Specular = vec4(0);
    }

    return Ambient + Diffuse + Specular * vec4(.7, .7,.7, 1.0);
}

void main()
{
    DirectionalLight d;
    d.Direction = directionalLight.Direction ;
    d.LightColor = directionalLight.LightColor;
    d.Energy = directionalLight.Energy;
    d.use = directionalLight.use;

    vec4 Light = CalculateDirectionalLight(d);
    FragColor = Light;
   
};