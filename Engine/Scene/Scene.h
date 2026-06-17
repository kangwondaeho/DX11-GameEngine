#pragma once

#include <memory>
#include <vector>
#include "GameObject.h"
#include "Camera.h"

class Scene
{
public:
    void Initialize(float aspectRatio);
    void Update(float deltaTime);

    GameObject* GetMainObject()
    {
        return mainObject;
    }

    Camera& GetMainCamera()
    {
        return mainCamera;
    }

private:
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    GameObject* mainObject = nullptr;

    Camera mainCamera;
};