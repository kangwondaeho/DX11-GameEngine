#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <DirectXMath.h>

#include "Mesh.h"

class D3D11Renderer
{
public:
    bool Initialize(HWND hwnd, int width, int height);

    void BeginFrame();

    const Mesh& GetCubeMesh() const
    {
        return cubeMesh;
    }

    const Mesh& GetPlaneMesh() const
    {
        return planeMesh;
    }

    void DrawMesh(
        const Mesh& mesh,
        DirectX::FXMMATRIX worldMatrix,
        DirectX::CXMMATRIX viewMatrix,
        DirectX::CXMMATRIX projectionMatrix,
        const DirectX::XMFLOAT4& color
    );

    void EndFrame();

private:
    bool CreateRenderTarget();
    bool CreateDepthStencilBuffer(int width, int height);
    bool CreateShaderResources();
    bool CreateConstantBuffer();
    bool CreateMaterialConstantBuffer();

private:
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> materialConstantBuffer;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

    Mesh cubeMesh;
    Mesh planeMesh;

    int renderWidth = 0;
    int renderHeight = 0;
};