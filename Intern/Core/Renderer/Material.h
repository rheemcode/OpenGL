#pragma once
#include "Math/SimpleVec.h"
#include "Renderer/Texture.h"
#include <memory>

struct Material
{
	struct
	{
		Vector4 Color;
		std::unique_ptr<Texture> Diffuse;
	};

	Vector3 Ambient;
	Vector3 Specular; 
	float Shininess;
	float SpecularHighlights;
	bool SSAO;

	Material()
		:Shininess(0), SpecularHighlights(0), SSAO(false){};
	Material(const Material& p_mat) = default;
};