#include "Rect.h"

Rect::Rect() : x(0), y(0), w(0), h(0) {}
Rect::Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

Vec2 Rect::GetCenter() const {
    return Vec2(x + (w / 2.0f), y + (h / 2.0f)); // 
}

bool Rect::Contains(const Vec2& p) const {
    // Verifica se o ponto p está dentro dos limites do retângulo [cite: 326]
    return (p.x >= x && p.x <= x + w && p.y >= y && p.y <= y + h);
}