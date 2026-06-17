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

    timer.Initialize();

    return true;
}

int Application::Run()
{
    while (window.ProcessMessages())
    {
        timer.Tick();

        const float deltaTime = timer.GetDeltaTime();

        renderer.Render(deltaTime);
    }

    return 0;
}