#include "Window.h"

#pragma comment(lib, "user32.lib")

bool Window::Create(HINSTANCE hInstance, int nCmdShow, int windowWidth, int windowHeight)
{
    width = windowWidth;
    height = windowHeight;

    const wchar_t CLASS_NAME[] = L"OpenXRDX11GameEngineWindowClass";

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = Window::WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(nullptr, L"RegisterClassEx failed", L"Error", MB_OK);
        return false;
    }

    RECT rect = { 0, 0, width, height };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"OpenXR-DX11-GameEngine",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd)
    {
        MessageBox(nullptr, L"CreateWindowEx failed", L"Error", MB_OK);
        return false;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    return true;
}

bool Window::ProcessMessages()
{
    MSG msg = {};

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}