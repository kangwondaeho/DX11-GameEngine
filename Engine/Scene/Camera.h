#pragma once

#include <DirectXMath.h>

class Camera
{
public:
    void SetPosition(float x, float y, float z);
    void SetTarget(float x, float y, float z);
    void SetPerspective(float fieldOfViewY, float aspectRatio, float nearZ, float farZ);

    DirectX::XMMATRIX GetViewMatrix() const;
    DirectX::XMMATRIX GetProjectionMatrix() const;

private:
    DirectX::XMFLOAT3 position = { 0.0f, 0.0f, -3.0f };
    DirectX::XMFLOAT3 target = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };

    float fieldOfViewY = DirectX::XM_PIDIV4;
    float aspectRatio = 16.0f / 9.0f;
    float nearZ = 0.1f;
    float farZ = 100.0f;
};