#pragma once
#include "Math/SimpleVec.h"
#include "OpenGL/Texture.h"

struct Material
{
	Vector3 Albedo;
	Vector3 Diffuse;
	uint32_t AlbedoTexture = 0;
	uint32_t SpecularTexture = 0;
	uint32_t NormalTexture = 0;
	bool NormalMapEnabled = false;
	Vector3 Specular;

	std::string AlbedoTexName = "";
	std::string SpecularTexName = "";
	std::string NormalTexName = "";

	float Shininess;
	float SpecularHighlight;
	bool useSSAO;

	Material()
		:Shininess(0), SpecularHighlight(0), useSSAO(false){};

	Material(const Material& p_mat) = default;
};

extern std::ostream& operator<<(std::ostream& os, Material& mat);
extern std::istream& operator>>(std::istream& is, Material& mat);

class ShaderMatrial
{

};