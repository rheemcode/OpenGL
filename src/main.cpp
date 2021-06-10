#include "Window/Window.h"
#include "Tests/Torus.h"
#include "Tests/Cube.h"

#define _USE_MATH_DEFINES

#define PI_ 3.14159265358979323846f


int main(void)
{
    Window window(700, 600);
    window.Init();

    Torus torus(10, 7, 4.f, 1.5);
    window.AddObject(torus);

    Cube cube;

    window.AddObject(cube);
    window.Update();
    return 0;
}
