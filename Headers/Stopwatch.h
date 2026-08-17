//
// Created by Mihnwq on 8/16/2026.
//
#pragma once
#include <chrono>

class Stopwatch
{
    std::chrono::steady_clock::time_point startTime;
    bool running = false;

public:
    void Start()
    {
        if (running)
            return;

        startTime = std::chrono::steady_clock::now();
        running = true;
    }

    double Stop()
    {
        if (!running)
            return 0.0;

        running = false;

        return std::chrono::duration<double>(
            std::chrono::steady_clock::now() - startTime
        ).count();
    }

    bool IsRunning() const
    {
        return running;
    }
};
