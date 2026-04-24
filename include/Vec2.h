#ifndef VEC2_H
#define VEC2_H

class Vec2 {
public:
    float x, y;
    Vec2();
    Vec2(float x, float y);
    
    // Funções recomendadas pelo PDF
    Vec2 operator+(const Vec2& v) const;
    Vec2 operator-(const Vec2& v) const;
    Vec2 operator*(const float scalar) const;

    float Magnitude() const;
    Vec2 Normalize() const;
    float GetAngle() const;              // Inclinação em relação ao eixo x
    Vec2 Rotate(float angle) const;      // Rotação baseada em matrizes
};

#endif