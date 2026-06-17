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

    input.Initialize(window.GetHandle());
    timer.Initialize();

    return true;
}

int Application::Run()
{
    while (window.ProcessMessages())
    {
        timer.Tick();

        const float deltaTime = timer.GetDeltaTime();

        if (window.WasResized())
        {
            int width = window.GetWidth();
            int height = window.GetHeight();

            if (renderer.Resize(width, height))
            {
                float aspectRatio =
                    static_cast<float>(width) / static_cast<float>(height);

                scene.GetMainCamera().SetPerspective(
                    DirectX::XM_PIDIV4,
                    aspectRatio,
                    0.1f,
                    100.0f
                );
            }

            window.ClearResizeFlag();
        }

        input.Update(window.GetHandle());

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
        camera.MoveForward(moveAmount);
    }

    if (input.IsKeyDown('S'))
    {
        camera.MoveForward(-moveAmount);
    }

    if (input.IsKeyDown('A'))
    {
        camera.MoveRight(-moveAmount);
    }

    if (input.IsKeyDown('D'))
    {
        camera.MoveRight(moveAmount);
    }

    if (input.IsKeyDown('Q'))
    {
        camera.MoveUp(-moveAmount);
    }

    if (input.IsKeyDown('E'))
    {
        camera.MoveUp(moveAmount);
    }

    if (input.IsKeyDown(VK_RBUTTON))
    {
        const float mouseSensitivity = 0.003f;

        const float yawDelta =
            static_cast<float>(input.GetMouseDeltaX()) * mouseSensitivity;

        const float pitchDelta =
            static_cast<float>(input.GetMouseDeltaY()) * -mouseSensitivity;

        camera.Rotate(yawDelta, pitchDelta);
    }
}