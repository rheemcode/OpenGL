#include <glpch.h>
#include "SkyBox.h"
#include <array>


SkyBox* SkyBox::s_Instance = nullptr;


void SkyBox::Init()
{
//    std::vector<float> vertices;
//    std::vector<uint32_t> vIndex;
//
//    int index = 0;
//
//    float sectorStep = (float)2.f * PI / 100;
//    float stackStep = float(PI / 100);
//
//    float lengthInv = 1.0f / 100;
//    float sectorAngle, stackAngle;
//    float z, xy;
//
//    for (int i = 0; i <= 100; ++i)
//    {
//        stackAngle = PI / 2.f - i * stackStep;
//        xy = 100 * Math::Cos(stackAngle);
//        z = 100 * Math::Sin(stackAngle);
//
//        for (int j = 0; j <= 100; ++j)
//        {
//            sectorAngle = j * sectorStep;
//            float x = xy * Math::Cos(sectorAngle);
//            float y = xy * Math::Sin(sectorAngle);
//
//            vertices.push_back(x);
//            vertices.push_back(y);
//            vertices.push_back(z);
//            vertices.push_back(z);
//            vertices.push_back(z);
////            a.vertices = { x, y, z };
//  //          a.normals = { x * lengthInv, y * lengthInv, z * lengthInv, };
//    //        a.uv = { (float)j / m_SectorSegments, (float)i / m_StackSegments };
//
//
//      //      attrib.push_back(a);
//        }
//
//    }
//    int k1, k2;
//    for (int i = 0; i < 100; i++)
//    {
//        k1 = i * (100 + 1);
//        k2 = k1 + 100 + 1;
//        for (int j = 0; j <= 100; j++)
//        {
//            if (i != 0)
//            {
//                vIndex.push_back(k1);
//                vIndex.push_back(k2);
//                vIndex.push_back(k1 + 1);
//            }
//
//            if (i != (100- 1))
//            {
//                vIndex.push_back(k1 + 1);
//                vIndex.push_back(k2);
//                vIndex.push_back(k2 + 1);
//            }
//
//            k1++;
//            k2++;
//        }
//    }

    float testRender[] = {
         -1.f, -1.f, -1.0f, 
         1.f,  -1.f, -1.0f, 
         1.f,   1.f,  -1.0f,
         -1.f,  1.f, -1.0f, 

    };

    uint32_t quadIndices[] = { 0, 1, 2, 2, 3, 0};

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
    m_vb = std::make_unique<VertexBuffer>(testRender, sizeof(testRender) * 4);
    m_vb->SetLayout({ { AttribDataType::T_FLOAT, Attrib::VERTEXPOSITION, AttribCount::VEC3, false }});
    m_vao->SetIndices(quadIndices, 6);
    m_vao->AddBuffer(*m_vb);

    m_shader = std::make_unique<Shader>("Assets/Shaders/skybox-procedural.glsl");
    //m_texture = std::make_unique<Texture>();


    //m_texture->AddCubeMapImage({ 
    //    "./Assets/Textures/dm.png", // back
    //    "./Assets/Textures/dm.png", // front
    //    "./Assets/Textures/dt.png", // right
    //    "./Assets/Textures/db.png", // left
    //    "./Assets/Textures/dm.png",  //top
    //    "./Assets/Textures/dm.png", }); //bottom
}

void SkyBox::BindTexture()
{
    //m_texture->BindCubeMap();
}

void SkyBox::Create()
{
	if (!s_Instance)
	{
		s_Instance = new SkyBox();
		s_Instance->Init();
	}
}
