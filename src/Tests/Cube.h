#pragma once
#include "Primitive.h"


class Cube : public Primitive
{
    void GenVertices();

public:
    Cube();
    virtual const Matrix4x4& GetTransform() const override;
    virtual const VertexArray& GetVertexAttribs() const override;
    virtual const Shader& GetShader() const override;
    virtual void OnUpdate() override;
};