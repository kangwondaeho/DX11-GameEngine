#include "Camera.h"

using namespace DirectX;

void Camera::SetPosition(float x, float y, float z)
{
    position = { x, y, z };
}

void Camera::SetTarget(float x, float y, float z)
{
    target = { x, y, z };
}

void Camera::SetPerspective(float fieldOfViewYValue, float aspectRatioValue, float nearZValue, float farZValue)
{
    fieldOfViewY = fieldOfViewYValue;
    aspectRatio = aspectRatioValue;
    nearZ = nearZValue;
    farZ = farZValue;
}

void Camera::Move(float x, float y, float z)
{
    position.x += x;
    position.y += y;
    position.z += z;

    target.x += x;
    target.y += y;
    target.z += z;
}

XMMATRIX Camera::GetViewMatrix() const
{
    XMVECTOR eyePosition = XMLoadFloat3(&position);
    XMVECTOR focusPosition = XMLoadFloat3(&target);
    XMVECTOR upDirection = XMLoadFloat3(&up);

    return XMMatrixLookAtLH(
        eyePosition,
        focusPosition,
        upDirection
    );
}

XMMATRIX Camera::GetProjectionMatrix() const
{
    return XMMatrixPerspectiveFovLH(
        fieldOfViewY,
        aspectRatio,
        nearZ,
        farZ
    );
}