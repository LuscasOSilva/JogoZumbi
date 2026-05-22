#include "Bullet.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include <cmath>

// A constante PI para a conversão de ângulos
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Bullet::Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance, std::string sprite) 
    : Component(associated) {
    
    // 1. Adiciona o componente visual da bala ao GameObject
    associated.AddComponent(new SpriteRenderer(associated, sprite));

    // 2. Salva as propriedades da bala
    this->damage = damage;
    this->distanceLeft = maxDistance;

    // 3. Decompõe a velocidade escalar nos eixos X e Y usando o ângulo (em radianos)
    this->speed.x = std::cos(angle) * speed;
    this->speed.y = std::sin(angle) * speed;

    // 4. A SDL usa graus para rotacionar a imagem, então convertemos radianos para graus
    associated.angleDeg = angle * 180.0 / M_PI;
}

void Bullet::Update(float dt) {
    // Calcula o deslocamento exato neste frame (velocidade * tempo)
    Vec2 displacement = speed * dt;
    
    // Move a "box" do GameObject
    associated.box.x += displacement.x;
    associated.box.y += displacement.y;

    // Subtrai a distância percorrida do total permitido (usando a magnitude do vetor)
    distanceLeft -= displacement.GetMagnitude();

    // Se a bala viajou mais do que devia, ela "morre"
    if (distanceLeft <= 0) {
        associated.RequestDelete();
    }
}

void Bullet::Render() {
    // Fica vazio, pois a imagem da bala já é desenhada pelo componente SpriteRenderer!
}

bool Bullet::Is(std::string type) {
    return type == "Bullet";
}

int Bullet::GetDamage() {
    return damage;
}