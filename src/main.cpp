#include "Window/Window.h"
#include "Tests/Torus.h"
#include "Tests/Cube.h"
#include "Tests/Sphere.h"


int main(void)
{
    Window window(700, 600);
    window.Init();
    Torus t = Torus(10, 10, 4.f, 1.5);
    Cube cube;
    Sphere sphere = Sphere(18, 36, 1.2f);
    window.AddObject(sphere);
    window.AddObject(t);
    window.AddObject(cube);
    window.Update();
    return 0;
}

