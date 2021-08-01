#pragma once

#include "Primitive.h"

class Sphere : public Primitive
{

    int m_SectorSegments = 0,
        m_StackSegments = 0,
        m_NumIndicies = 0,
        m_RestartIndex = 0,
        m_NumVertices = 0;

    float m_radius = 0;

public:
    //void Draw(const class Camera& camera) override;
    void Rotate();
    void Scale();

    void GenSphereVertices();
    void GenIndicies();
    virtual void OnUpdate() override {};

    Sphere(int stackSegments, int sectorSegments, float radius);
    ~Sphere();


};

