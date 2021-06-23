//#include "Window/Window.h"
//#include "Tests/Torus.h"
//#include "Tests/Cube.h"
//#include "Tests/Sphere.h"
#include <Windows.h>
#include <Camera.h>
#include <Window/Window.h>
#include <Tests/Torus.h>
#include <Tests/Cube.h>
#include <Tests/Sphere.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lparam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"OpenGL Window Class";

    WNDCLASS wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"OpenGL Test",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 700, 600,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    
   

    if (hwnd == NULL)
        return EXIT_FAILURE;
    ShowWindow(hwnd, nCmdShow);
    HWND hwnd2 = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Test",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        hwnd, NULL, hInstance, NULL

    );
    if (hwnd2 == NULL)
        return EXIT_FAILURE;

    ShowWindow(hwnd2, nCmdShow);
    MSG msg = {};


    Window window(700, 600);
    window.Init(hwnd);
    Torus t = Torus(10, 10, 4.f, 1.5);
    Cube cube;
    Sphere sphere = Sphere(18, 36, 1.2f);
    window.AddObject(sphere);
    window.AddObject(t);
    window.AddObject(cube);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        window.Update();

    }

    return EXIT_SUCCESS;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lparam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return EXIT_SUCCESS;
        default:
              return DefWindowProc(hwnd, uMsg, wParam, lparam);
    }
}