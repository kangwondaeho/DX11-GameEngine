#include "Scene.h"
#include "RotatorComponent.h"

#include <DirectXMath.h>

using namespace DirectX;

void Scene::Initialize(float aspectRatio)
{
    mainCamera.SetPosition(0.0f, 0.0f, -3.0f);
    mainCamera.SetTarget(0.0f, 0.0f, 0.0f);
    mainCamera.SetPerspective(XM_PIDIV4, aspectRatio, 0.1f, 100.0f);

    auto cube = std::make_unique<GameObject>();

    cube->GetTransform().position = { 0.0f, 0.0f, 0.0f };
    cube->GetTransform().scale = { 1.0f, 1.0f, 1.0f };

    cube->AddComponent<RotatorComponent>(0.75f, 1.5f, 0.0f);

    mainObject = cube.get();

    gameObjects.push_back(std::move(cube));
}

void Scene::Update(float deltaTime)
{
    for (auto& gameObject : gameObjects)
    {
        gameObject->Update(deltaTime);
    }
}