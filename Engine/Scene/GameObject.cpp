#include "GameObject.h"

void GameObject::Update(float deltaTime)
{
    if (!active)
    {
        return;
    }

    for (auto& component : components)
    {
        if (!component->IsEnabled())
        {
            continue;
        }

        component->StartInternal();
        component->Update(deltaTime);
    }
}