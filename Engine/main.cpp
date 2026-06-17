#include <Windows.h>
#include "Core/Application.h"

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow)
{
    Application app;

    if (!app.Initialize(hInstance, nCmdShow))
    {
        return -1;
    }

    return app.Run();
}