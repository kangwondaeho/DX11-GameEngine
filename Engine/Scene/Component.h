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

    void SetEnabled(bool newEnabled)
    {
        enabled = newEnabled;
    }

    bool IsEnabled() const
    {
        return enabled;
    }

    void StartInternal()
    {
        if (started)
        {
            return;
        }

        Start();
        started = true;
    }

    virtual void Start() {}
    virtual void Update(float deltaTime) {}

protected:
    GameObject* owner = nullptr;

private:
    bool started = false;
    bool enabled = true;
};