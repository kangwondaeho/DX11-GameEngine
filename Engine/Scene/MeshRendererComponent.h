#pragma once

#include "Component.h"
#include <DirectXMath.h>

class Mesh;

class MeshRendererComponent : public Component
{
public:
    MeshRendererComponent() = default;

    explicit MeshRendererComponent(
        const Mesh* mesh,
        const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f })
        : mesh(mesh), color(color)
    {
    }

    void SetMesh(const Mesh* newMesh)
    {
        mesh = newMesh;
    }

    const Mesh* GetMesh() const
    {
        return mesh;
    }

    void SetColor(const DirectX::XMFLOAT4& newColor)
    {
        color = newColor;
    }

    const DirectX::XMFLOAT4& GetColor() const
    {
        return color;
    }

    void SetVisible(bool newVisible)
    {
        visible = newVisible;
    }

    bool IsVisible() const
    {
        return visible;
    }

private:
    const Mesh* mesh = nullptr;
    DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

    bool visible = true;
};