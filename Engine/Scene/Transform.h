#pragma once

#include <DirectXMath.h>

struct Transform
{
    DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };

    DirectX::XMMATRIX GetWorldMatrix() const
    {
        using namespace DirectX;

        XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);

        XMMATRIX rotationMatrix =
            XMMatrixRotationX(rotation.x) *
            XMMatrixRotationY(rotation.y) *
            XMMatrixRotationZ(rotation.z);

        XMMATRIX translationMatrix =
            XMMatrixTranslation(position.x, position.y, position.z);

        return scaleMatrix * rotationMatrix * translationMatrix;
    }
};