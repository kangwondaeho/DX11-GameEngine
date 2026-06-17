#include "D3D11Renderer.h"

#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

struct Vertex
{
    float position[3];
    float color[4];
};

struct TransformConstantBuffer
{
    XMMATRIX worldViewProjection;
};

bool D3D11Renderer::Initialize(HWND hwnd, int width, int height)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0
    };

    D3D_FEATURE_LEVEL selectedFeatureLevel = {};

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        featureLevels,
        1,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        swapChain.GetAddressOf(),
        device.GetAddressOf(),
        &selectedFeatureLevel,
        context.GetAddressOf()
    );

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"D3D11CreateDeviceAndSwapChain failed", L"Error", MB_OK);
        return false;
    }

    if (!CreateRenderTarget())
    {
        return false;
    }

    if (!CreateDepthStencilBuffer(width, height))
    {
        return false;
    }

    context->OMSetRenderTargets(
        1,
        renderTargetView.GetAddressOf(),
        depthStencilView.Get()
    );

    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewport);

    renderWidth = width;
    renderHeight = height;

    if (!CreateCubeResources())
    {
        return false;
    }

    if (!CreateConstantBuffer())
    {
        return false;
    }

    return true;
}

bool D3D11Renderer::CreateRenderTarget()
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

    HRESULT hr = swapChain->GetBuffer(
        0,
        __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(backBuffer.GetAddressOf())
    );

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"SwapChain GetBuffer failed", L"Error", MB_OK);
        return false;
    }

    hr = device->CreateRenderTargetView(
        backBuffer.Get(),
        nullptr,
        renderTargetView.GetAddressOf()
    );

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"CreateRenderTargetView failed", L"Error", MB_OK);
        return false;
    }

    return true;
}

bool CompileShaderFromFile(
    const wchar_t* fileName,
    const char* entryPoint,
    const char* target,
    Microsoft::WRL::ComPtr<ID3DBlob>& shaderBlob)
{
    UINT compileFlags = 0;

#ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG;
    compileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

    HRESULT hr = D3DCompileFromFile(
        fileName,
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint,
        target,
        compileFlags,
        0,
        shaderBlob.GetAddressOf(),
        errorBlob.GetAddressOf()
    );

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            MessageBoxA(
                nullptr,
                static_cast<const char*>(errorBlob->GetBufferPointer()),
                "Shader Compile Error",
                MB_OK
            );
        }
        else
        {
            MessageBoxW(
                nullptr,
                fileName,
                L"Shader File Not Found or Compile Failed",
                MB_OK
            );
        }

        return false;
    }

    return true;
}

bool D3D11Renderer::CreateCubeResources()
{
    Vertex vertices[] =
    {
        // 앞면
        { { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { {  0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { {  0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        { { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } },

        // 뒷면
        { { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
        { {  0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
        { {  0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
        { { -0.5f, -0.5f,  0.5f }, { 0.2f, 0.2f, 0.2f, 1.0f } },
    };

    UINT indices[] =
    {
        // 앞면
        0, 1, 2,
        0, 2, 3,

        // 뒷면
        4, 6, 5,
        4, 7, 6,

        // 왼쪽
        4, 0, 3,
        4, 3, 7,

        // 오른쪽
        1, 5, 6,
        1, 6, 2,

        // 위
        4, 5, 1,
        4, 1, 0,

        // 아래
        3, 2, 6,
        3, 6, 7
    };

    indexCount = static_cast<UINT>(indexCount = sizeof(indices) / sizeof(UINT));

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
        MessageBox(nullptr, L"Create vertex buffer failed", L"Error", MB_OK);
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
        MessageBox(nullptr, L"Create index buffer failed", L"Error", MB_OK);
        return false;
    }

    Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob;

    if (!CompileShaderFromFile(
        L"Shaders/Basic.hlsl",
        "VSMain",
        "vs_5_0",
        vertexShaderBlob))
    {
        return false;
    }

    if (!CompileShaderFromFile(
        L"Shaders/Basic.hlsl",
        "PSMain",
        "ps_5_0",
        pixelShaderBlob))
    {
        return false;
    }

    hr = device->CreateVertexShader(
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        nullptr,
        vertexShader.GetAddressOf()
    );

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"Create vertex shader failed", L"Error", MB_OK);
        return false;
    }

    hr = device->CreatePixelShader(
        pixelShaderBlob->GetBufferPointer(),
        pixelShaderBlob->GetBufferSize(),
        nullptr,
        pixelShader.GetAddressOf()
    );

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"Create pixel shader failed", L"Error", MB_OK);
        return false;
    }

    D3D11_INPUT_ELEMENT_DESC inputElements[] =
    {
        {
            "POSITION",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            0,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
        {
            "COLOR",
            0,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            0,
            12,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        }
    };

    hr = device->CreateInputLayout(
        inputElements,
        2,
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        inputLayout.GetAddressOf()
    );

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"Create input layout failed", L"Error", MB_OK);
        return false;
    }

    return true;
}

void D3D11Renderer::Render()
{
    const float clearColor[4] = { 0.1f, 0.15f, 0.25f, 1.0f };

    context->ClearRenderTargetView(
        renderTargetView.Get(),
        clearColor
    );

    context->ClearDepthStencilView(
        depthStencilView.Get(),
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
        1.0f,
        0
    );

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    context->IASetInputLayout(inputLayout.Get());

    context->IASetVertexBuffers(
        0,
        1,
        vertexBuffer.GetAddressOf(),
        &stride,
        &offset
    );

    context->IASetIndexBuffer(
        indexBuffer.Get(),
        DXGI_FORMAT_R32_UINT,
        0
    );

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->VSSetShader(vertexShader.Get(), nullptr, 0);
    context->PSSetShader(pixelShader.Get(), nullptr, 0);

    rotationAngle += 0.01f;

    float aspectRatio = static_cast<float>(renderWidth) / static_cast<float>(renderHeight);

    XMMATRIX world = XMMatrixRotationY(rotationAngle) * XMMatrixRotationX(rotationAngle * 0.5f);

    XMVECTOR eyePosition = XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f);
    XMVECTOR focusPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMMATRIX view = XMMatrixLookAtLH(
        eyePosition,
        focusPosition,
        upDirection
    );

    XMMATRIX projection = XMMatrixPerspectiveFovLH(
        XM_PIDIV4,
        aspectRatio,
        0.1f,
        100.0f
    );

    TransformConstantBuffer transformData = {};
    transformData.worldViewProjection = XMMatrixTranspose(world * view * projection);

    context->UpdateSubresource(
        constantBuffer.Get(),
        0,
        nullptr,
        &transformData,
        0,
        0
    );

    context->VSSetConstantBuffers(
        0,
        1,
        constantBuffer.GetAddressOf()
    );

    context->DrawIndexed(indexCount, 0, 0);

    swapChain->Present(1, 0);
}

bool D3D11Renderer::CreateDepthStencilBuffer(int width, int height)
{
    D3D11_TEXTURE2D_DESC depthBufferDesc = {};
    depthBufferDesc.Width = width;
    depthBufferDesc.Height = height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    HRESULT hr = device->CreateTexture2D(
        &depthBufferDesc,
        nullptr,
        depthStencilBuffer.GetAddressOf()
    );

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"Create depth stencil buffer failed", L"Error", MB_OK);
        return false;
    }

    hr = device->CreateDepthStencilView(
        depthStencilBuffer.Get(),
        nullptr,
        depthStencilView.GetAddressOf()
    );

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"Create depth stencil view failed", L"Error", MB_OK);
        return false;
    }

    return true;
}

bool D3D11Renderer::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = sizeof(TransformConstantBuffer);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    HRESULT hr = device->CreateBuffer(
        &bufferDesc,
        nullptr,
        constantBuffer.GetAddressOf()
    );

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"Create constant buffer failed", L"Error", MB_OK);
        return false;
    }

    return true;
}