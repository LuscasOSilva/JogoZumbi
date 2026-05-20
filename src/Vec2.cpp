#include "Vec2.h"
#include <cmath>

Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(float x, float y) : x(x), y(y) {}

Vec2 Vec2::operator+(const Vec2& rhs) const {
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2& rhs) const {
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator*(const float rhs) const {
    return Vec2(x * rhs, y * rhs);
}

float Vec2::GetMagnitude() const {
    return std::sqrt((x * x) + (y * y));
}

Vec2 Vec2::GetNormalized() const {
    float mag = GetMagnitude();
    if (mag == 0) return Vec2(0, 0); // Previne divisão por zero
    return Vec2(x / mag, y / mag);
}

float Vec2::Distance(const Vec2& target) const {
    Vec2 diff = target - *this;
    return diff.GetMagnitude();
}

float Vec2::GetAngle() const {
    // atan2 resolve os sinais automaticamente nos 4 quadrantes
    return std::atan2(y, x); 
}

float Vec2::GetAngle(const Vec2& target) const {
    Vec2 diff = target - *this;
    return diff.GetAngle();
}

void Vec2::Rotate(float angle) {
    float oldX = x;
    float oldY = y;
    // Matriz de rotação 2D
    x = oldX * std::cos(angle) - oldY * std::sin(angle);
    y = oldX * std::sin(angle) + oldY * std::cos(angle);
}