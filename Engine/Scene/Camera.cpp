#include "Camera.h"

#include <algorithm>
#include <cmath>

using namespace DirectX;

void Camera::SetPosition(float x, float y, float z)
{
    position = { x, y, z };
}

void Camera::SetTarget(float x, float y, float z)
{
    XMVECTOR eye = XMLoadFloat3(&position);
    XMVECTOR target = XMVectorSet(x, y, z, 0.0f);

    XMVECTOR direction = XMVector3Normalize(target - eye);

    XMFLOAT3 dir = {};
    XMStoreFloat3(&dir, direction);

    yaw = std::atan2(dir.x, dir.z);
    pitch = std::asin(dir.y);
}

void Camera::SetPerspective(float fieldOfViewYValue, float aspectRatioValue, float nearZValue, float farZValue)
{
    fieldOfViewY = fieldOfViewYValue;
    aspectRatio = aspectRatioValue;
    nearZ = nearZValue;
    farZ = farZValue;
}

void Camera::MoveForward(float distance)
{
    XMVECTOR forward = GetForwardVector();
    XMVECTOR pos = XMLoadFloat3(&position);

    pos += forward * distance;

    XMStoreFloat3(&position, pos);
}

void Camera::MoveRight(float distance)
{
    XMVECTOR right = GetRightVector();
    XMVECTOR pos = XMLoadFloat3(&position);

    pos += right * distance;

    XMStoreFloat3(&position, pos);
}

void Camera::MoveUp(float distance)
{
    position.y += distance;
}

void Camera::Rotate(float yawDelta, float pitchDelta)
{
    yaw += yawDelta;
    pitch += pitchDelta;

    const float pitchLimit = XM_PIDIV2 - 0.01f;

    pitch = std::clamp(pitch, -pitchLimit, pitchLimit);
}

XMVECTOR Camera::GetForwardVector() const
{
    const float cosPitch = std::cos(pitch);
    const float sinPitch = std::sin(pitch);
    const float cosYaw = std::cos(yaw);
    const float sinYaw = std::sin(yaw);

    return XMVector3Normalize(
        XMVectorSet(
            sinYaw * cosPitch,
            sinPitch,
            cosYaw * cosPitch,
            0.0f
        )
    );
}

XMVECTOR Camera::GetRightVector() const
{
    XMVECTOR forward = GetForwardVector();
    XMVECTOR worldUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    return XMVector3Normalize(XMVector3Cross(worldUp, forward));
}

XMMATRIX Camera::GetViewMatrix() const
{
    XMVECTOR eyePosition = XMLoadFloat3(&position);
    XMVECTOR forward = GetForwardVector();
    XMVECTOR focusPosition = eyePosition + forward;
    XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

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