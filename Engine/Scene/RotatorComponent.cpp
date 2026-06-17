#include "RotatorComponent.h"
#include "GameObject.h"

#include <Windows.h>

RotatorComponent::RotatorComponent(float xSpeed, float ySpeed, float zSpeed)
    : xSpeed(xSpeed), ySpeed(ySpeed), zSpeed(zSpeed)
{
}

void RotatorComponent::Start()
{
}

void RotatorComponent::Update(float deltaTime)
{
    if (!owner)
    {
        return;
    }

    Transform& transform = owner->GetTransform();

    transform.rotation.x += xSpeed * deltaTime;
    transform.rotation.y += ySpeed * deltaTime;
    transform.rotation.z += zSpeed * deltaTime;
}