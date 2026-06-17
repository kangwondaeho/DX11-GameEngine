#include "Input.h"

bool Input::IsKeyDown(int virtualKey) const
{
    return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}