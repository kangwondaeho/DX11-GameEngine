#include "Application.h"
#include "../Scene/MeshRendererComponent.h"

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

        Camera& camera = scene.GetMainCamera();

        renderer.BeginFrame();

        for (const auto& gameObject : scene.GetGameObjects())
        {
            if (!gameObject->GetComponent<MeshRendererComponent>())
            {
                continue;
            }

            renderer.DrawCube(
                gameObject->GetTransform().GetWorldMatrix(),
                camera.GetViewMatrix(),
                camera.GetProjectionMatrix()
            );
        }

        renderer.EndFrame();
    }

    return 0;
}