#include <Windows.h>
#include "Core/Window.h"
#include "Renderer/D3D11Renderer.h"

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow)
{
    Window window;

    if (!window.Create(hInstance, nCmdShow, 1280, 720))
    {
        return -1;
    }

    D3D11Renderer renderer;

    if (!renderer.Initialize(window.GetHandle(), 1280, 720))
    {
        return -1;
    }

    while (window.ProcessMessages())
    {
        renderer.Render();
    }

    return 0;
}