#include "GameObject.h"

void GameObject::Update(float deltaTime)
{
    if (!active)
    {
        return;
    }

    for (auto& component : components)
    {
        component->StartInternal();
        component->Update(deltaTime);
    }
}