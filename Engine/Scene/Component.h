#pragma once

class GameObject;

class Component
{
public:
    virtual ~Component() = default;

    void SetOwner(GameObject* ownerObject)
    {
        owner = ownerObject;
    }

    virtual void Update(float deltaTime) {}

protected:
    GameObject* owner = nullptr;
};