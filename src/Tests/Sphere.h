#pragma once

#include "Primitive.h"

class Sphere : public Primitive
{
public:
    Sphere(const Sphere& t);
    Sphere(int stackSegments, int sectorSegments, float radius);
    ~Sphere();

public:
    void Draw(const class Camera& camera) override;
    void Rotate();
    void Scale();

private:
    void GenSphereVertices();
    void GenIndicies();

    void EnableAttribs() override;
    void CreateVertexArray() override;
    void CreateVertexBuffer() override;
    void CreateIndexBuffer() override;


private:
    std::vector<vec4> m_VertexPositions;
    int m_SectorSegments,
        m_StackSegments,
        m_NumIndicies,
        m_RestartIndex,
        m_NumVertices;

    float m_radius;

    int m_MvpLocation;
    int m_UniformLocation;
};

