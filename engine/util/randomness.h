#pragma once

#include <random>
#include <stdexcept>
#include <algorithm>

static std::mt19937 random_engine{std::random_device{}()};

// Uniform random int within [min, max] inclusive
int randint(int min, int max);

// Returns true at the given percentage rate. Useful for
// probabilistically making decisions:
// if (probability(30)) {
//     // 30% chance
// }
// else {
//     // 70% chance
// }
bool probability(int percentage);


// Choose an random element from the given container
template <typename T>
auto& random_choice(T& container) {
    if (container.empty()) {
        throw std::runtime_error("Container is empty");
    }
    else if (container.size() == 1) {
        return *std::begin(container);
    }
    else {
        int index = randint(0, container.size()-1);
        auto it = std::next(std::begin(container), index);
        return *it;
    }
}

// Randomly shuffle the elements between begin and end
template <typename Iterator>
void shuffle(Iterator begin, Iterator end) {
    std::shuffle(begin, end, random_engine);
}
