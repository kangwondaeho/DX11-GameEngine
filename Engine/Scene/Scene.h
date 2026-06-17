#pragma once

#include <memory>
#include <vector>
#include <string>

#include "GameObject.h"
#include "Camera.h"

class Mesh;

class Scene
{
public:
    void Initialize(float aspectRatio, const Mesh* cubeMesh, const Mesh* planeMesh);
    void Update(float deltaTime);

    GameObject* CreateGameObject(const std::string& name = "GameObject");
    GameObject* FindGameObjectByName(const std::string& name);

    const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const
    {
        return gameObjects;
    }

    Camera& GetMainCamera()
    {
        return mainCamera;
    }

private:
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    Camera mainCamera;
};