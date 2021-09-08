#shader vertex
#version 330 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos * 10, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D uvTex;
uniform vec2 iMouse;
uniform float iTime;

#define BOXSIZE vec2(1, 1)
#define iResolution vec2(1200, 700)

float sdRoundBox(in vec2 p, in vec2 b, in vec4 r)
{
	r.xy = (p.x > 0.0) ? r.xy : r.zw;
	r.x = (p.y > 0.0) ? r.x : r.y;
	
	vec2 q = abs(p) - b + r.x;
	return min( max( q.x, q.y ), 0.0) + length( max( q, 0.0 )) - r.x;
}


void main()
{
	vec2 p = (2.0 * gl_FragCoord.xy - iResolution) / iResolution.y;
	vec2 m = (2.0 * iMouse - iResolution) / iResolution.y;

	vec4 ra = 0.3 + 0.3 * cos(2.0 * iTime + vec4(0, 1, 2, 3));
	float d = sdRoundBox(p, BOXSIZE, ra);

	vec3 col = vec3(1.0) - sign(d) * vec3(0.1, 0.4, 0.7);
	col *= 1.0 - exp( -3.0 * abs(d));
	col *= 0.8 + 0.2 * cos(150.0 * d);
	col = mix( col, vec3(1.0), 1.0 - smoothstep(0.0, 0.01, abs(d)));

	FragColor = vec4(col, 1.0);
}