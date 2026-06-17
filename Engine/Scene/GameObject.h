#pragma once

#include <memory>
#include <vector>
#include <utility>
#include "Transform.h"
#include "Component.h"

class GameObject
{
public:
    Transform& GetTransform()
    {
        return transform;
    }

    const Transform& GetTransform() const
    {
        return transform;
    }

    template <typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T* rawPointer = component.get();

        component->SetOwner(this);
        components.push_back(std::move(component));

        return rawPointer;
    }

    template <typename T>
    T* GetComponent()
    {
        for (auto& component : components)
        {
            if (T* casted = dynamic_cast<T*>(component.get()))
            {
                return casted;
            }
        }

        return nullptr;
    }

    void Update(float deltaTime);

private:
    Transform transform;
    std::vector<std::unique_ptr<Component>> components;
};