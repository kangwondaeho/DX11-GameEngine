#include "CameraControllerComponent.h"

#include "Camera.h"
#include "../Core/Input.h"

#include <Windows.h>

CameraControllerComponent::CameraControllerComponent(Camera* camera, Input* input)
    : camera(camera), input(input)
{
}

void CameraControllerComponent::Update(float deltaTime)
{
    if (!camera || !input)
    {
        return;
    }

    const float moveAmount = moveSpeed * deltaTime;

    if (input->IsKeyDown('W'))
    {
        camera->MoveForward(moveAmount);
    }

    if (input->IsKeyDown('S'))
    {
        camera->MoveForward(-moveAmount);
    }

    if (input->IsKeyDown('A'))
    {
        camera->MoveRight(-moveAmount);
    }

    if (input->IsKeyDown('D'))
    {
        camera->MoveRight(moveAmount);
    }

    if (input->IsKeyDown('Q'))
    {
        camera->MoveUp(-moveAmount);
    }

    if (input->IsKeyDown('E'))
    {
        camera->MoveUp(moveAmount);
    }

    if (input->IsKeyDown(VK_RBUTTON))
    {
        const float yawDelta =
            static_cast<float>(input->GetMouseDeltaX()) * mouseSensitivity;

        const float pitchDelta =
            static_cast<float>(input->GetMouseDeltaY()) * -mouseSensitivity;

        camera->Rotate(yawDelta, pitchDelta);
    }
}