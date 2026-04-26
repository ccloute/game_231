#include "timer.h"

Timer::Timer()
    :previous{std::chrono::high_resolution_clock::now()} {}

double Timer::elapsed() {
    auto current = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> difference{current - previous};
    previous = current;
    return difference.count();
}
