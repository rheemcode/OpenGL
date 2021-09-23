#shader vertex
#version 410 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}

#shader fragment
#version 410 core

out vec4 FragColor;
  
in vec2 TexCoords;
#define iResolution vec2(1200, 700)
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;


void main()
{             
    // retrieve data from G-buffer
    vec3 Color;
    vec2 Coords = (2 * gl_FragCoord.xy - iResolution) / iResolution.y;
    Coords.x /= 2;
    if (TexCoords.x > 0.5)
        Color = texture(gNormal, Coords).rgb;
    else if (TexCoords.x < 0.5)
        Color = texture(gPosition, Coords).rgb;
    if (TexCoords.y > 0.5)
        Color = texture(gAlbedoSpec, Coords).rgb;
//    {
//        Coords.x =  0.5f;
//    }
  // vec3 FragPos = texture(gNormal, Coords).rgb;
  // if (TexCoords.x > 0.5)
   //    discard;
   // vec3 Normal = texture(gNormal, TexCoords).rgb;
   // vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
   // float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    FragColor = vec4(Color, 1.0);
}  