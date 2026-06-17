#include "Timer.h"

void Timer::Initialize()
{
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&previousTime);

    deltaTime = 0.0f;
    totalTime = 0.0f;
}

void Timer::Tick()
{
    QueryPerformanceCounter(&currentTime);

    const double elapsedCounts =
        static_cast<double>(currentTime.QuadPart - previousTime.QuadPart);

    const double elapsedSeconds =
        elapsedCounts / static_cast<double>(frequency.QuadPart);

    deltaTime = static_cast<float>(elapsedSeconds);
    totalTime += deltaTime;

    previousTime = currentTime;
}