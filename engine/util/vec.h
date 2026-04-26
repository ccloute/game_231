#pragma once

#include <ostream>
#include <array>

// 2D vector class
class Vec {
public:
    int x{0}, y{0};
};

bool operator<(const Vec& a, const Vec& b);
bool operator==(const Vec& a, const Vec& b);
bool operator!=(const Vec& a, const Vec& b);

Vec operator+(const Vec& a, const Vec& b);
Vec& operator+=(Vec& a, const Vec& b);
Vec operator-(const Vec& a, const Vec& b);
Vec& operator-=(Vec& a, const Vec& b);
Vec operator*(const Vec& v, int scalar);
Vec operator*(int scalar, const Vec& v);
Vec operator/(const Vec& v, int scalar);

std::ostream& operator<<(std::ostream& os, const Vec& vec);

double distance(const Vec& a, const Vec& b);
Vec unit(const Vec& v);

// degrees in image/compass coordinates: north = 0, east = 90, etc
double angle(const Vec& v);

// Hash function for storing Vec's in an unordered_set/map
namespace std {
    template<> struct [[maybe_unused]] hash<Vec> {
        static void hash_combine(std::size_t& seed, int value) {
            // from boost hash_combine docs
            seed ^= hash<int>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }

        size_t operator()(const Vec& p) const {
            std::size_t seed = 0;
            hash_combine(seed, p.x);
            hash_combine(seed, p.y);
            return seed;
        }
    };
}

// Cartesian directions: right, up, left, down
constexpr std::array<Vec, 4> Directions = {Vec{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
