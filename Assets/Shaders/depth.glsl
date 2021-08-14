#shader vertex
#version 410 core


layout(location = 0) in vec3 vPos;

uniform mat4 projView;
uniform mat4 model;

void main()  
{  
     gl_Position = projView * model * vec4(vPos, 1.0);
};



#shader fragment
#version 410 core

//out vec4 FragColor;

 void main()  
 {  
   gl_FragDepth = gl_FragCoord.z;
 };