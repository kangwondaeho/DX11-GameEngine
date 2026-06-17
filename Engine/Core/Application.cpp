#include "Application.h"

bool Application::Initialize(HINSTANCE hInstance, int nCmdShow)
{
    if (!window.Create(hInstance, nCmdShow, WindowWidth, WindowHeight))
    {
        return false;
    }

    if (!renderer.Initialize(window.GetHandle(), WindowWidth, WindowHeight))
    {
        return false;
    }

    return true;
}

int Application::Run()
{
    while (window.ProcessMessages())
    {
        renderer.Render();
    }

    return 0;
}