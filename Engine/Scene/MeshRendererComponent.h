#pragma once

#include "Component.h"

class Mesh;

class MeshRendererComponent : public Component
{
public:
    MeshRendererComponent() = default;

    explicit MeshRendererComponent(const Mesh* mesh)
        : mesh(mesh)
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

private:
    const Mesh* mesh = nullptr;
};