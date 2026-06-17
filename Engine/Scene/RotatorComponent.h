#pragma once

#include "Component.h"

class RotatorComponent : public Component
{
public:
    RotatorComponent(float xSpeed, float ySpeed, float zSpeed);

    void Update(float deltaTime) override;

private:
    float xSpeed = 0.0f;
    float ySpeed = 0.0f;
    float zSpeed = 0.0f;
};