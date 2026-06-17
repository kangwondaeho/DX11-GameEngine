#include "Mesh.h"

bool Mesh::CreateCube(ID3D11Device* device)
{
    Vertex vertices[] =
    {
        // ¾Õ¸é
        { { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { {  0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { {  0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        { { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } },

        // µÞ¸é
        { { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
        { {  0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
        { {  0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
        { { -0.5f, -0.5f,  0.5f }, { 0.2f, 0.2f, 0.2f, 1.0f } },
    };

    unsigned int indices[] =
    {
        // ¾Õ¸é
        0, 1, 2,
        0, 2, 3,

        // µÞ¸é
        4, 6, 5,
        4, 7, 6,

        // ¿ÞÂÊ
        4, 0, 3,
        4, 3, 7,

        // ¿À¸¥ÂÊ
        1, 5, 6,
        1, 6, 2,

        // À§
        4, 5, 1,
        4, 1, 0,

        // ¾Æ·¡
        3, 2, 6,
        3, 6, 7
    };

    indexCount = sizeof(indices) / sizeof(unsigned int);

    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = sizeof(vertices);
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = vertices;

    HRESULT hr = device->CreateBuffer(
        &vertexBufferDesc,
        &vertexData,
        vertexBuffer.GetAddressOf()
    );

    if (FAILED(hr))
    {
        return false;
    }

    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.ByteWidth = sizeof(indices);
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices;

    hr = device->CreateBuffer(
        &indexBufferDesc,
        &indexData,
        indexBuffer.GetAddressOf()
    );

    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

void Mesh::Draw(ID3D11DeviceContext* context) const
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    ID3D11Buffer* vertexBuffers[] = { vertexBuffer.Get() };

    context->IASetVertexBuffers(
        0,
        1,
        vertexBuffers,
        &stride,
        &offset
    );

    context->IASetIndexBuffer(
        indexBuffer.Get(),
        DXGI_FORMAT_R32_UINT,
        0
    );

    context->DrawIndexed(indexCount, 0, 0);
}

bool Mesh::CreatePlane(ID3D11Device* device)
{
    Vertex vertices[] =
    {
        // position                  // color
        { { -2.5f, 0.0f,  2.5f }, { 0.4f, 0.4f, 0.4f, 1.0f } },
        { {  2.5f, 0.0f,  2.5f }, { 0.4f, 0.4f, 0.4f, 1.0f } },
        { {  2.5f, 0.0f, -2.5f }, { 0.4f, 0.4f, 0.4f, 1.0f } },
        { { -2.5f, 0.0f, -2.5f }, { 0.4f, 0.4f, 0.4f, 1.0f } },
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    indexCount = sizeof(indices) / sizeof(unsigned int);

    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = sizeof(vertices);
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = vertices;

    HRESULT hr = device->CreateBuffer(
        &vertexBufferDesc,
        &vertexData,
        vertexBuffer.GetAddressOf()
    );

    if (FAILED(hr))
    {
        return false;
    }

    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.ByteWidth = sizeof(indices);
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices;

    hr = device->CreateBuffer(
        &indexBufferDesc,
        &indexData,
        indexBuffer.GetAddressOf()
    );

    if (FAILED(hr))
    {
        return false;
    }

    return true;
}