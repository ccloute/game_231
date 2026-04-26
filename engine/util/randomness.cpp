#include "randomness.h"

int randint(int min, int max) {
    if (min >= max) {
        std::string msg{"min must be less than max: randint("};
        msg += std::to_string(min) + ", " + std::to_string(max) + ")";
        throw std::runtime_error(msg);
    }
    std::uniform_int_distribution dist{min, max}; // [min, max] inclusive
    return dist(random_engine);
}

bool probability(int percentage) {
    if (percentage < 0) {
        throw std::runtime_error("percentage must be positive: " + std::to_string(percentage));
    }
    return randint(0, 99) < percentage;
}
