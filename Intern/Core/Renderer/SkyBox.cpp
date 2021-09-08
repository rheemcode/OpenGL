#include <glpch.h>
#include "SkyBox.h"
#include <array>


SkyBox* SkyBox::s_Instance = nullptr;


void SkyBox::Init()
{
    constexpr float SIZE = 500;
    Vector3 vertexCoords[24]{
        // Back
        {  SIZE, -SIZE, -SIZE },
        {-SIZE, -SIZE, -SIZE},
        { -SIZE, SIZE, -SIZE}, 
        {SIZE, SIZE, -SIZE},

        // Front
        { -SIZE, -SIZE, SIZE},
        {SIZE, -SIZE, SIZE},
        {SIZE, SIZE, SIZE},
        {-SIZE, SIZE, SIZE},

        // Right
        {SIZE, -SIZE, SIZE},
        {SIZE, -SIZE, -SIZE},
        {SIZE, SIZE, -SIZE},
        {SIZE, SIZE, SIZE },

        // Left
        {-SIZE, -SIZE, -SIZE},
        {-SIZE, -SIZE, SIZE},
        { -SIZE, SIZE, SIZE},
        {-SIZE,SIZE, -SIZE},

        // Top
        {-SIZE, SIZE, SIZE},
        {SIZE, SIZE, SIZE},
        {SIZE, SIZE, -SIZE },
        {-SIZE, SIZE,-SIZE },

        // Bottom
        {-SIZE, -SIZE,-SIZE},
        { SIZE, -SIZE, -SIZE},
        {SIZE, -SIZE, SIZE},
        {-SIZE, -SIZE, SIZE}
    };

    uint32_t indices[] = { 0, 1, 2, 2, 3, 0, 4,  5,  6,  6,  7,  4,
                          8,  9,  10, 10, 11, 8,12, 13, 14, 14, 15, 12,
                          16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20 };

    m_vao = std::make_unique<VertexArray>();
    m_vb = std::make_unique<VertexBuffer>(vertexCoords, 24 * sizeof(Vector3));
    m_vb->SetLayout({ {AttribDataType::T_FLOAT, Attrib::VERTEXPOSITION, AttribCount::VEC3, false } }); 
    m_vao->SetIndices(indices, 36);
    m_vao->AddBuffer(*m_vb);

    m_shader = std::make_unique<Shader>("Assets/Shaders/skybox.glsl");
    m_texture = std::make_unique<Texture>();


    m_texture->AddCubeMapImage({ 
        "./Assets/Textures/dm.png", // back
        "./Assets/Textures/dm.png", // front
        "./Assets/Textures/dt.png", // right
        "./Assets/Textures/db.png", // left
        "./Assets/Textures/dm.png",  //top
        "./Assets/Textures/dm.png", }); //bottom
}

void SkyBox::BindTexture()
{
    m_texture->BindCubeMap();
}

void SkyBox::Create()
{
	if (!s_Instance)
	{
		s_Instance = new SkyBox();
		s_Instance->Init();
	}
}
