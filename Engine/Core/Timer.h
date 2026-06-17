#pragma once

#include <Windows.h>

class Timer
{
public:
    void Initialize();
    void Tick();

    float GetDeltaTime() const { return deltaTime; }
    float GetTotalTime() const { return totalTime; }

private:
    LARGE_INTEGER frequency = {};
    LARGE_INTEGER previousTime = {};
    LARGE_INTEGER currentTime = {};

    float deltaTime = 0.0f;
    float totalTime = 0.0f;
};