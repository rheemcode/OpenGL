#shader [vertex]
#version 430 core


layout (location = 0) in vec3 aPos;

layout(std140, binding = 0) uniform Matrices
{
   mat4 view;
   mat4 proj; // offset 0
   mat4 model; // offset 16 * 4
   mat4 shadowSpaceMatrix[4]; //  128
};

void main()
{
    gl_Position = model * vec4(aPos, 1.0);
}

#shader [geometry]
#version 430 core


layout (triangles) in;
layout (triangle_strip, max_vertices=12) out;

#define MAX_SPLIT 4
//
layout(std140, binding = 0) uniform Matrices
{
   mat4 view;
   mat4 proj; // offset 0
   mat4 model; // offset 16 * 4
   mat4 shadowSpaceMatrix[MAX_SPLIT]; //  128
};


out vec4 FragPos;

void main()
{
    for(int face = 0; face < 4; ++face)
    {
        gl_Layer = face;
        for(int i = 0; i < 3; ++i)
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadowSpaceMatrix[face] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
} 

#shader [fragment]
#version 430 core

in vec4 FragPos;

void main()
{
    // write this as modified depth
   // gl_FragDepth = -FragPos.z;
}