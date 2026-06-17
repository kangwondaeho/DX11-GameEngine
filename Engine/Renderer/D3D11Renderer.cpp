#include "D3D11Renderer.h"
#include <cstring>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

struct Vertex
{
    float position[3];
    float color[4];
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

    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewport);

    if (!CreateQuadResources())
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

    context->OMSetRenderTargets(
        1,
        renderTargetView.GetAddressOf(),
        nullptr
    );

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

bool D3D11Renderer::CreateQuadResources()
{
    Vertex vertices[] =
    {
        // position                  // color
        { { -0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // 0: 왼쪽 위
        { {  0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // 1: 오른쪽 위
        { {  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // 2: 오른쪽 아래
        { { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // 3: 왼쪽 아래
    };

    UINT indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    indexCount = 6;

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

    context->DrawIndexed(indexCount, 0, 0);

    swapChain->Present(1, 0);
}