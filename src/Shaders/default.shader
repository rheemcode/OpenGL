#shader vertex
#version 410 core


layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texIndex;

layout(location = 0) out vec2 TexCoord; 
layout(location = 1) out float TexIndex; 

uniform mat4 viewProjection;

void main()  
{  
     gl_Position = viewProjection * vec4(vPos, 1.0);
     TexCoord = texCoord;
     TexIndex = texIndex;
};



#shader fragment
#version 410 core

layout(location = 0) in vec2 TexCoord;
layout(location = 1) in float TexIndex;

uniform sampler2D tex[32];



out vec4 FragColor;

 void main()  
 {  
     switch (int(TexIndex))
     {
        case 1: FragColor = texture(tex[0], TexCoord); break;
        case 2: FragColor = texture(tex[1], TexCoord); break;
        case 3: FragColor = texture(tex[2], TexCoord); break;
      }
 //  FragColor = texture(tex, TexCoord);
 };