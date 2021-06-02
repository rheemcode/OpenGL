#shader vertex

#version 110 
uniform mat4 MVP; 
attribute vec4 vCol; 
attribute vec4 vPos; 
varying vec4 m_color; 

void main()  
{  
     gl_Position = MVP * vPos;  
     m_color = vCol;  
};



#shader fragment
#version 110
uniform vec4 u_Color;
varying vec4 m_color;  

 void main()  
 {  
     gl_FragColor = u_Color;
 };