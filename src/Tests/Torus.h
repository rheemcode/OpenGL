#include "Primitive.h"

class Torus : public Primitive
{
public:
    Torus(const Torus& t);
    Torus(int mainSegments, int tubeSegments, float mainRadius, float tubeRadius);
    ~Torus();

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
    std::vector<vec4> m_VertexPositions;
    int m_MainSegments,
        m_TubeSegments,
        m_NumIndicies,
        m_RestartIndex,
        m_NumVertices;

    float m_MainRadius, m_TubeRadius;

    int m_MvpLocation;
    int m_UniformLocation;
};

