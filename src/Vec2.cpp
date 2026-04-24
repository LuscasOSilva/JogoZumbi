#include "Vec2.h"
#include <cmath>

Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(float x, float y) : x(x), y(y) {}

Vec2 Vec2::operator+(const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
Vec2 Vec2::operator-(const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
Vec2 Vec2::operator*(const float scalar) const { return Vec2(x * scalar, y * scalar); }

float Vec2::Magnitude() const {
    return std::sqrt(x * x + y * y);
}

Vec2 Vec2::Normalize() const {
    float mag = Magnitude();
    return (mag != 0) ? (*this * (1.0f / mag)) : Vec2(0, 0);
}

float Vec2::GetAngle() const {
    return std::atan2(y, x);
}

Vec2 Vec2::Rotate(float angle) const {
    // Fórmulas de rotação do PDF 
    return Vec2(
        x * std::cos(angle) - y * std::sin(angle),
        y * std::cos(angle) + x * std::sin(angle)
    );
}