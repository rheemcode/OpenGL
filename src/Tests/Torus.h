#include "Primitive.h"

class Torus : public Primitive
{
public:
    Torus(int mainSegments, int tubeSegments, float mainRadius, float tubeRadius);


public:
    void Draw(const class Camera& camera) override;
    void Rotate();
    void Scale();

private:
    void GenTorusVertices();
    void GenIndicies();

    void EnableAttribs() override;
    void CreateVertexArray() override;
    void CreateVertexBuffer() override;
    void CreateIndexBuffer() override;


private:
    int m_MainSegments,
        m_TubeSegments,
        m_NumIndicies,
        m_RestartIndex,
        m_NumVertices;

    float m_MainRadius, m_TubeRadius;

    int m_MvpLocation;
    int m_UniformLocation;
};

