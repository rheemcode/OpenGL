#shader vertex
#version 410 core


layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 texCoord;
layout(location = 0) out vec2 TexCoord; 



void main()  
{  
     gl_Position = vec4(vPos, 1.0);  
     TexCoord = texCoord;
};



#shader fragment
#version 410 core

layout(location = 0) in vec2 texCoord;
uniform sampler2D tex;

out vec4 FragColor;

 void main()  
 {  
     FragColor = texture(tex, texCoord);
 };