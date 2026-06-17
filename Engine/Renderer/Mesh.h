#pragma once

#include <d3d11.h>
#include <wrl/client.h>

struct Vertex
{
    float position[3];
    float color[4];
};

class Mesh
{
public:
    bool CreateCube(ID3D11Device* device);
    bool CreatePlane(ID3D11Device* device);

    void Draw(ID3D11DeviceContext* context) const;

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

    unsigned int indexCount = 0;
};