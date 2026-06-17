#include "Scene.h"
#include "RotatorComponent.h"
#include "MeshRendererComponent.h"

#include <DirectXMath.h>

using namespace DirectX;

GameObject* Scene::CreateGameObject(const std::string& name)
{
    auto gameObject = std::make_unique<GameObject>(name);

    GameObject* rawPointer = gameObject.get();

    gameObjects.push_back(std::move(gameObject));

    return rawPointer;
}

void Scene::Initialize(float aspectRatio, const Mesh* cubeMesh, const Mesh* planeMesh)
{
    mainCamera.SetPosition(0.0f, 1.5f, -6.0f);
    mainCamera.SetTarget(0.0f, 0.0f, 0.0f);
    mainCamera.SetPerspective(XM_PIDIV4, aspectRatio, 0.1f, 100.0f);

    GameObject* cube1 = CreateGameObject("Cube1");
    cube1->GetTransform().position = { -1.5f, 0.0f, 0.0f };
    cube1->GetTransform().scale = { 1.0f, 1.0f, 1.0f };
    cube1->AddComponent<RotatorComponent>(0.75f, 1.5f, 0.0f);
    cube1->AddComponent<MeshRendererComponent>(
        cubeMesh,
        XMFLOAT4(1.0f, 0.4f, 0.4f, 1.0f)
    );

    GameObject* cube2 = CreateGameObject("Cube2");
    cube2->GetTransform().position = { 1.5f, 0.0f, 0.0f };
    cube2->GetTransform().scale = { 0.75f, 0.75f, 0.75f };
    cube2->AddComponent<RotatorComponent>(1.2f, 0.5f, 0.0f);
    cube2->AddComponent<MeshRendererComponent>(
        cubeMesh,
        XMFLOAT4(0.4f, 0.8f, 1.0f, 1.0f)
    );

    GameObject* cube3 = CreateGameObject("Cube3");
    cube3->GetTransform().position = { 0.0f, 1.3f, 0.0f };
    cube3->GetTransform().scale = { 0.5f, 0.5f, 0.5f };
    cube3->AddComponent<RotatorComponent>(0.3f, 1.0f, 1.5f);
    cube3->AddComponent<MeshRendererComponent>(
        cubeMesh,
        XMFLOAT4(0.6f, 1.0f, 0.5f, 1.0f)
    );

    GameObject* floor = CreateGameObject("Floor");
    floor->GetTransform().position = { 0.0f, -1.0f, 0.0f };
    floor->GetTransform().scale = { 1.0f, 1.0f, 1.0f };
    floor->AddComponent<MeshRendererComponent>(
        planeMesh,
        XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f)
    );
}

void Scene::Update(float deltaTime)
{
    for (auto& gameObject : gameObjects)
    {
        gameObject->Update(deltaTime);
    }
}

GameObject* Scene::FindGameObjectByName(const std::string& name)
{
    for (auto& gameObject : gameObjects)
    {
        if (gameObject->GetName() == name)
        {
            return gameObject.get();
        }
    }

    return nullptr;
}