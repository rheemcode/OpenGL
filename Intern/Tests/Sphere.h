#pragma once

#include "Primitive.h"

class Sphere : public Primitive
{

    int m_SectorSegments,
        m_StackSegments,
        m_NumIndicies,
        m_RestartIndex,
        m_NumVertices;

    float m_radius;

    int m_MvpLocation;
    int m_UniformLocation;

public:
    //void Draw(const class Camera& camera) override;
    void Rotate();
    void Scale();

    void GenSphereVertices();
    virtual const Matrix4x4& GetTransform() const override;
    virtual const VertexArray& GetVertexAttribs() const override;
    virtual const Shader& GetShader() const override;
    void GenIndicies();
    virtual void OnUpdate() override {};
    Sphere(const Sphere& t);
    Sphere(int stackSegments, int sectorSegments, float radius);
    ~Sphere();


};

