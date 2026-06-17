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

    scene.Initialize(
        aspectRatio,
        &renderer.GetCubeMesh(),
        &renderer.GetPlaneMesh()
    );
    timer.Initialize();

    return true;
}

int Application::Run()
{
    while (window.ProcessMessages())
    {
        timer.Tick();

        const float deltaTime = timer.GetDeltaTime();

        HandleInput(deltaTime);

        scene.Update(deltaTime);

        Camera& camera = scene.GetMainCamera();

        renderer.BeginFrame();

        for (const auto& gameObject : scene.GetGameObjects())
        {
            MeshRendererComponent* meshRenderer =
                gameObject->GetComponent<MeshRendererComponent>();

            if (!meshRenderer)
            {
                continue;
            }

            const Mesh* mesh = meshRenderer->GetMesh();

            if (!mesh)
            {
                continue;
            }

            renderer.DrawMesh(
                *mesh,
                gameObject->GetTransform().GetWorldMatrix(),
                camera.GetViewMatrix(),
                camera.GetProjectionMatrix(),
                meshRenderer->GetColor()
            );
        }

        renderer.EndFrame();
    }

    return 0;
}

void Application::HandleInput(float deltaTime)
{
    Camera& camera = scene.GetMainCamera();

    const float moveSpeed = 3.0f;
    const float moveAmount = moveSpeed * deltaTime;

    if (input.IsKeyDown('W'))
    {
        camera.Move(0.0f, 0.0f, moveAmount);
    }

    if (input.IsKeyDown('S'))
    {
        camera.Move(0.0f, 0.0f, -moveAmount);
    }

    if (input.IsKeyDown('A'))
    {
        camera.Move(-moveAmount, 0.0f, 0.0f);
    }

    if (input.IsKeyDown('D'))
    {
        camera.Move(moveAmount, 0.0f, 0.0f);
    }

    if (input.IsKeyDown('Q'))
    {
        camera.Move(0.0f, -moveAmount, 0.0f);
    }

    if (input.IsKeyDown('E'))
    {
        camera.Move(0.0f, moveAmount, 0.0f);
    }
}