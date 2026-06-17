#pragma once

#include "Component.h"

class Camera;
class Input;

class CameraControllerComponent : public Component
{
public:
    CameraControllerComponent(Camera* camera, Input* input);

    void Update(float deltaTime) override;

private:
    Camera* camera = nullptr;
    Input* input = nullptr;

    float moveSpeed = 3.0f;
    float mouseSensitivity = 0.003f;
};