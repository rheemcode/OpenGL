#shader vertex
#version 410 core

layout(location = 0) in vec3 vPos;

layout(location = 0) out vec3 TexCoord; 

uniform mat4 projView;

void main()  
{  
     gl_Position = projView * vec4(vPos, 1.0);
     TexCoord = vPos;
};



#shader fragment
#version 410 core

layout(location = 0) in vec3 TexCoord;

uniform samplerCube cubeMap;


vec3 brightnessContrast(vec3 value, float brightness, float contrast)
{
    return (value - 0.5) * contrast + 0.5 + (brightness-1);
}

vec3 gamma(vec3 value, float param)
{
    return vec3(pow(abs(value.r), param),pow(abs(value.g), param),pow(abs(value.b), param));
}


out vec4 FragColor;
vec4 color;

 void main()  
 {  
     color = texture(cubeMap, TexCoord);
  //  color = vec4(brightnessContrast(color.xyz, 1.15f, 1.15f), color.w);
  //  color = vec4(gamma(color.xyz, 4.8f),color.w);
	FragColor = color;
 };