#pragma once
#include "Math/SimpleVec.h"
#include "Renderer/Texture.h"
#include <memory>

struct Material
{
	struct
	{
		Vector4 Color;
		int Diffuse;
	};

	Vector3 Ambient;
	Vector3 Specular; 
	float Shininess;
	float SpecularHighlights;
	bool SSAO;

	Material()
		:Shininess(0), SpecularHighlights(0), SSAO(false){};
	Material(const Material& p_mat)
	{
		Color = p_mat.Color;
		Diffuse = p_mat.Diffuse;
		Ambient = p_mat.Ambient;
		Shininess = p_mat.Shininess;
		SpecularHighlights = p_mat.Shininess;
		SSAO = p_mat.SSAO;

	}
};