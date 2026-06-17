#pragma once

#include <Windows.h>

class Window
{
public:
    bool Create(HINSTANCE hInstance, int nCmdShow, int width, int height);
    bool ProcessMessages();

    HWND GetHandle() const { return hwnd; }

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

    bool WasResized() const { return resized; }
    void ClearResizeFlag() { resized = false; }

    void SetTitle(const wchar_t* title);

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

private:
    HWND hwnd = nullptr;

    int width = 0;
    int height = 0;

    bool resized = false;
};