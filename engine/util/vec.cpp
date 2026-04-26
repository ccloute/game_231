#include "vec.h"
#include <cmath>
#include <numbers>

// lexicographic ordering: compare x's, then compare y's
bool operator<(const Vec& a, const Vec& b) {
    return (a.x < b.x) || ((a.x == b.x) && (a.y < b.y));
}

bool operator==(const Vec& a, const Vec& b) {
    return a.x == b.x && a.y == b.y;
}

bool operator!=(const Vec& a, const Vec& b) {
    return !(a == b);
}

Vec operator+(const Vec& a, const Vec& b) {
    return Vec{a.x+b.x, a.y+b.y};
}

Vec& operator+=(Vec& a, const Vec& b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}

Vec operator-(const Vec& a, const Vec& b) {
    return Vec{a.x-b.x, a.y-b.y};
}

Vec& operator-=(Vec& a, const Vec& b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

Vec operator*(const Vec& v, int scalar) {
    return Vec{v.x*scalar, v.y*scalar};
}

Vec operator*(int scalar, const Vec& v) {
    return v*scalar;
}

Vec operator/(const Vec& v, int scalar) {
    if (scalar == 0) {
        throw std::runtime_error("Cannot divide a Vec by zero");
    }
    return Vec{v.x/scalar, v.y/scalar};
}

std::ostream& operator<<(std::ostream& os, const Vec& vec) {
    return os << "(" << vec.x << ", " << vec.y << ")";
}

double distance(const Vec& a, const Vec& b) {
    auto [dx, dy] = a - b;
    return std::sqrt(dx*dx + dy*dy);
}

Vec unit(const Vec& v) {
    double length = std::sqrt(v.x*v.x + v.y*v.y);
    return v / length;
}

double angle(const Vec& direction) {
    double degrees_unit_circle = atan2(direction.y, direction.x) / std::numbers::pi * 180;
    double degrees_compass = fmod(450 - degrees_unit_circle, 360);
    return degrees_compass;
}
