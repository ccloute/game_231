#pragma once

#include <chrono>

// High-resolution timer
class Timer {
public:
    Timer();

    // Time since last call to elapsed() or constructor
    double elapsed();
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> previous;
};
