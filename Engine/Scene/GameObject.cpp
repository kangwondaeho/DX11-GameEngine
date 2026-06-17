#include "GameObject.h"

void GameObject::Update(float deltaTime)
{
    for (auto& component : components)
    {
        component->StartInternal();
        component->Update(deltaTime);
    }
}