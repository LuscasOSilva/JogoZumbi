#ifndef VEC2_H
#define VEC2_H

class Vec2 {
public:
    float x, y;

    // Construtores
    Vec2();
    Vec2(float x, float y);
    
    // Funções recomendadas pelo PDF, Sobrecarga de operadores matemáticos (permite fazer vec1 + vec2)
    Vec2 operator+(const Vec2& rhs) const;
    Vec2 operator-(const Vec2& rhs) const;
    Vec2 operator*(const float rhs) const;

    // Operações vetoriais
    float GetMagnitude() const; // Tamanho do vetor
    Vec2 GetNormalized() const; // Retorna o vetor com tamanho 1 (útil para direções)

    // Distância entre dois pontos
    float Distance(const Vec2& target) const;

    // Inclinação do vetor em radianos (ângulo com o eixo X)
    float GetAngle() const;

    // Ângulo da reta que liga este ponto ao ponto alvo (em radianos)
    float GetAngle(const Vec2& target) const;

    // Roda este vetor num dado ângulo (em radianos)
    void Rotate(float angle);
};

#endif