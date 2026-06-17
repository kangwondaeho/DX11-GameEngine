#include "Scene.h"
#include "RotatorComponent.h"
#include "MeshRendererComponent.h"

#include <DirectXMath.h>

using namespace DirectX;

void Scene::Initialize(float aspectRatio, const Mesh* defaultMesh)
{
    mainCamera.SetPosition(0.0f, 0.0f, -5.0f);
    mainCamera.SetTarget(0.0f, 0.0f, 0.0f);
    mainCamera.SetPerspective(XM_PIDIV4, aspectRatio, 0.1f, 100.0f);

    auto cube1 = std::make_unique<GameObject>();
    cube1->GetTransform().position = { -1.5f, 0.0f, 0.0f };
    cube1->GetTransform().scale = { 1.0f, 1.0f, 1.0f };
    cube1->AddComponent<RotatorComponent>(0.75f, 1.5f, 0.0f);
    cube1->AddComponent<MeshRendererComponent>(defaultMesh);
    gameObjects.push_back(std::move(cube1));

    auto cube2 = std::make_unique<GameObject>();
    cube2->GetTransform().position = { 1.5f, 0.0f, 0.0f };
    cube2->GetTransform().scale = { 0.75f, 0.75f, 0.75f };
    cube2->AddComponent<RotatorComponent>(1.2f, 0.5f, 0.0f);
    cube2->AddComponent<MeshRendererComponent>(defaultMesh);
    gameObjects.push_back(std::move(cube2));

    auto cube3 = std::make_unique<GameObject>();
    cube3->GetTransform().position = { 0.0f, 1.3f, 0.0f };
    cube3->GetTransform().scale = { 0.5f, 0.5f, 0.5f };
    cube3->AddComponent<RotatorComponent>(0.3f, 1.0f, 1.5f);
    cube3->AddComponent<MeshRendererComponent>(defaultMesh);
    gameObjects.push_back(std::move(cube3));
}

void Scene::Update(float deltaTime)
{
    for (auto& gameObject : gameObjects)
    {
        gameObject->Update(deltaTime);
    }
}