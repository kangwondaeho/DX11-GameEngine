#include "Scene.h"
#include "RotatorComponent.h"

void Scene::Initialize()
{
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