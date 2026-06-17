#pragma once

#include <memory>
#include <vector>
#include "GameObject.h"

class Scene
{
public:
    void Initialize();
    void Update(float deltaTime);

    GameObject* GetMainObject()
    {
        return mainObject;
    }

private:
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    GameObject* mainObject = nullptr;
};