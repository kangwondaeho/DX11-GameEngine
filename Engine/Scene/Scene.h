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