#include <Windows.h>
#include "Core/Window.h"

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

    while (window.ProcessMessages())
    {
        // 아직 DirectX 없음
        // 나중에 Update(), Render() 들어갈 자리
    }

    return 0;
}