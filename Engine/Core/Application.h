#pragma once

#include <Windows.h>
#include "Window.h"
#include "Timer.h"
#include "../Renderer/D3D11Renderer.h"
#include "../Scene/Scene.h"

class Application
{
public:
    bool Initialize(HINSTANCE hInstance, int nCmdShow);
    int Run();

private:
    Window window;
    D3D11Renderer renderer;
    Timer timer;
    Scene scene;

    static constexpr int WindowWidth = 1280;
    static constexpr int WindowHeight = 720;
};