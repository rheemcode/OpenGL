#pragma once

class Renderer
{
public:
    
    enum PRIMITIVE_MODE
    {
        TRIANGLES,
        TRIANGLE_FAN,
        TRIANGLE_STRIP,
        LINES
    };

    void BeginScene();
    void RenderScene(){}
    void EndScene(){}

};
