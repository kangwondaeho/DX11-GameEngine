#pragma once

#include <Windows.h>

class Input
{
public:
    void Initialize(HWND hwnd);
    void Update(HWND hwnd);

    bool IsKeyDown(int virtualKey) const;

    int GetMouseDeltaX() const { return mouseDeltaX; }
    int GetMouseDeltaY() const { return mouseDeltaY; }

private:
    POINT previousMousePosition = {};
    POINT currentMousePosition = {};

    int mouseDeltaX = 0;
    int mouseDeltaY = 0;

    bool initialized = false;
};