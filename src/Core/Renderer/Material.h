#pragma once
#include "Math/SimpleVec.h"
#include "Renderer/Texture.h"
#include <memory>

struct Material
{
	struct
	{
		Vector4 Color;
		std::unique_ptr<Texture> texture;
	};
	
	float Shininess;
	float SpecularHighlights;
	bool SSAO;
};