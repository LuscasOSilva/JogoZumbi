#ifndef RECT_H
#define RECT_H

#include "Vec2.h"

class Rect {
public:
    float x, y, w, h;

    Rect();
    Rect(float x, float y, float w, float h);

    Vec2 GetCenter() const; // 
    bool Contains(const Vec2& p) const; // [cite: 326]
};

#endif