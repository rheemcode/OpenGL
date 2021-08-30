#shader vertex
#version 410 core


layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texIndex;

struct VertexOutput
{
    vec2 TexCoord;
    float TexIndex;
};

layout(location = 0) out VertexOutput Output;

uniform mat4 viewProjection;

void main()
{
    gl_Position = vec4(vPos, 1.0);
    Output.TexCoord = texCoord;
    Output.TexIndex = texIndex;
};



#shader fragment
#version 410 core

struct VertexOutput
{
    vec2 TexCoord;
    float TexIndex;
};

layout(location = 0) in VertexOutput Input;
uniform sampler2D u_Textures;

out vec4 FragColor;

void main()
{

	FragColor = texture(u_Textures, Input.TexCoord);
};