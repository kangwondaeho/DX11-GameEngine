#include "D3D11Renderer.h"

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

    if (!CreateTriangleResources())
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

bool D3D11Renderer::CreateTriangleResources()
{
    Vertex vertices[] =
    {
        { {  0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
    };

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

    const char* shaderSource = R"(
        struct VSInput
        {
            float3 position : POSITION;
            float4 color : COLOR;
        };

        struct PSInput
        {
            float4 position : SV_POSITION;
            float4 color : COLOR;
        };

        PSInput VSMain(VSInput input)
        {
            PSInput output;
            output.position = float4(input.position, 1.0f);
            output.color = input.color;
            return output;
        }

        float4 PSMain(PSInput input) : SV_TARGET
        {
            return input.color;
        }
    )";

    Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

    hr = D3DCompile(
        shaderSource,
        strlen(shaderSource),
        nullptr,
        nullptr,
        nullptr,
        "VSMain",
        "vs_5_0",
        0,
        0,
        vertexShaderBlob.GetAddressOf(),
        errorBlob.GetAddressOf()
    );

    if (FAILED(hr))
    {
        MessageBoxA(nullptr, static_cast<char*>(errorBlob->GetBufferPointer()), "Vertex Shader Error", MB_OK);
        return false;
    }

    hr = D3DCompile(
        shaderSource,
        strlen(shaderSource),
        nullptr,
        nullptr,
        nullptr,
        "PSMain",
        "ps_5_0",
        0,
        0,
        pixelShaderBlob.GetAddressOf(),
        errorBlob.ReleaseAndGetAddressOf()
    );

    if (FAILED(hr))
    {
        MessageBoxA(nullptr, static_cast<char*>(errorBlob->GetBufferPointer()), "Pixel Shader Error", MB_OK);
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

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->VSSetShader(vertexShader.Get(), nullptr, 0);
    context->PSSetShader(pixelShader.Get(), nullptr, 0);

    context->Draw(3, 0);

    swapChain->Present(1, 0);
}