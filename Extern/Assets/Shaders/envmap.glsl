#shader vertex

#version 410 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 normal;

out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	Normal = mat3(transpose(inverse(model))) * normal;
	Position = vec3(model * vec4(vPos, 1.0));
	gl_Position = proj  * view * vec4(Position, 1.0);

}

#shader fragment
#version 410 core

out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 camPos;
uniform samplerCube skybox;

void main()
{
	float ratio = 1.00 / 1.303;
	vec3 I = normalize(Position - camPos);
	vec3 R = refract(I, normalize(Normal), ratio);
	vec3 R2 = refract(R, normalize(Normal), ratio);
	vec3 R3 = reflect(I, normalize(Normal));
	FragColor = vec4(texture(skybox, R3).rgb, 1.0);
}