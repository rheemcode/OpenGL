#include "Window/Window.h"
#include "Tests/Torus.h"
#include "Tests/Cube.h"



int main(void)
{
    Window window(700, 600);
    window.Init();
    Torus t = Torus(10, 10, 4.f, 1.5);
    window.AddObject(t);
    window.Update();
    return 0;
}

