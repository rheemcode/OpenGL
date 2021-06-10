#shader vertex

#version 330 core

attribute vec4 vCol; 
attribute vec4 vPos; 
varying vec4 m_color; 

uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 modelMatrix;

void main()  
{  
     gl_Position = projMatrix * viewMatrix * modelMatrix * vPos;  
     m_color = vCol;  
};



#shader fragment
#version 330 core
uniform vec4 u_Color;
varying vec4 m_color;  

 void main()  
 {  
     gl_FragColor = u_Color;
 };