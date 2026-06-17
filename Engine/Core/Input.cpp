#include "Input.h"

void Input::Initialize(HWND hwnd)
{
    GetCursorPos(&previousMousePosition);
    ScreenToClient(hwnd, &previousMousePosition);

    currentMousePosition = previousMousePosition;

    mouseDeltaX = 0;
    mouseDeltaY = 0;

    initialized = true;
}

void Input::Update(HWND hwnd)
{
    if (!initialized)
    {
        Initialize(hwnd);
        return;
    }

    GetCursorPos(&currentMousePosition);
    ScreenToClient(hwnd, &currentMousePosition);

    mouseDeltaX = currentMousePosition.x - previousMousePosition.x;
    mouseDeltaY = currentMousePosition.y - previousMousePosition.y;

    previousMousePosition = currentMousePosition;
}

bool Input::IsKeyDown(int virtualKey) const
{
    return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}