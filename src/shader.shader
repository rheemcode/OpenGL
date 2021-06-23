#shader vertex

#version 410 core

//attribute vec4 vCol; 
layout(location = 0) in vec4 vPos; 
out vec4 m_color; 

uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 modelMatrix;

void main()  
{  
     gl_Position = projMatrix * viewMatrix * modelMatrix * vPos;  
     m_color = vec4(1.0f, 1.0f, 0, 1.0f);  
};



#shader fragment
#version 410 core

uniform vec4 u_Color;
in vec4 m_color;  
out vec4 outPutCol;

 void main()  
 {  
     outPutCol = m_color;
 };