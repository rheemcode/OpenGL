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


//uniform vec4 objectColor;
uniform vec4 AmbientColor;
uniform vec4 LightColor;
uniform vec3 LightPosition;
uniform vec3 ViewPosition;
uniform float AmbientStrength;
uniform float Shininess;


out vec4 FragColor;

vec4 CalculateLighting()
{
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(LightPosition - FragPos);
    vec3 viewDir = normalize(ViewPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float specular = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float diffuse = max(dot(normal, lightDir), 0.0);

    vec4 Diffuse = diffuse * LightColor;
    vec4 Ambient = AmbientStrength * AmbientColor;
    vec4 Specular = Shininess * specular * LightColor;
    vec4 res = (Ambient + Diffuse + Specular) * vec4(1, 0, 0, 1.0);
    return res;
}

 void main()  
 {  
     vec4 Light = CalculateLighting();
    FragColor = Light;
 };