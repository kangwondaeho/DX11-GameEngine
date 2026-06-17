#include "Application.h"

#include "../Scene/MeshRendererComponent.h"
#include "../Scene/Camera.h"

#include <DirectXMath.h>
#include <string>

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

    input.Initialize(window.GetHandle());

    const float aspectRatio =
        static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight);

    scene.Initialize(
        aspectRatio,
        &renderer.GetCubeMesh(),
        &renderer.GetPlaneMesh(),
        &input
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

        UpdateWindowTitle(deltaTime);

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

        scene.Update(deltaTime);

        RenderScene();
    }

    return 0;
}

void Application::UpdateWindowTitle(float deltaTime)
{
    fpsUpdateTimer += deltaTime;
    frameCount++;

    if (fpsUpdateTimer >= 1.0f)
    {
        const float fps = static_cast<float>(frameCount) / fpsUpdateTimer;

        std::wstring title =
            L"OpenXR DX11 GameEngine - FPS: " +
            std::to_wstring(static_cast<int>(fps));

        window.SetTitle(title.c_str());

        fpsUpdateTimer = 0.0f;
        frameCount = 0;
    }
}

void Application::RenderScene()
{
    Camera& camera = scene.GetMainCamera();

    DirectX::XMMATRIX viewMatrix = camera.GetViewMatrix();
    DirectX::XMMATRIX projectionMatrix = camera.GetProjectionMatrix();

    renderer.BeginFrame();

    for (const auto& gameObject : scene.GetGameObjects())
    {
        MeshRendererComponent* meshRenderer =
            gameObject->GetComponent<MeshRendererComponent>();

        if (!meshRenderer)
        {
            continue;
        }

        if (!meshRenderer->IsVisible())
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
            viewMatrix,
            projectionMatrix,
            meshRenderer->GetColor()
        );
    }

    renderer.EndFrame();
}