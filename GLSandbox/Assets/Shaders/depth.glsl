#shader vertex
#version 330 core


layout (location = 0) in vec3 aPos;


uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = (lightSpaceMatrix * model) * vec4(aPos, 1.0);
    
}

//#shader geometry
//#version 410 core
//
//layout (triangles) in;
//layout (triangle_strip, max_vertices=256) out;
//
//
//void main()
//{             
//    for (int i = 0; i < gl_in.length(); ++i)
//    {
//        gl_Layer = 0;
//        gl_Position = gl_in[i].gl_Position;
//        EmitVertex();
//    }
//    EndPrimitive();
//    // gl_FragDepth = gl_FragCoord.z;
//}
//
//#shader fragment
//#version 410 core
//
//out vec4 FragColor;
//
//void main()
//{               		
//    FragColor = vec4(gl_FragCoord.z,0,0, 1.0);
//    // gl_FragDepth = gl_FragCoord.z;
//}