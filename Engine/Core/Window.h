#pragma once

#include <Windows.h>

class Window
{
public:
    bool Create(HINSTANCE hInstance, int nCmdShow, int width, int height);
    bool ProcessMessages();

    HWND GetHandle() const { return hwnd; }

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    HWND hwnd = nullptr;
    int width = 0;
    int height = 0;
};