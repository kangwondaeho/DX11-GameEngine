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

    const float aspectRatio =
        static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight);

    scene.Initialize(aspectRatio);
    timer.Initialize();

    return true;
}

int Application::Run()
{
    while (window.ProcessMessages())
    {
        timer.Tick();

        const float deltaTime = timer.GetDeltaTime();

        scene.Update(deltaTime);

        GameObject* mainObject = scene.GetMainObject();

        if (mainObject)
        {
            Camera& camera = scene.GetMainCamera();

            renderer.Render(
                mainObject->GetTransform().GetWorldMatrix(),
                camera.GetViewMatrix(),
                camera.GetProjectionMatrix()
            );
        }
    }

    return 0;
}