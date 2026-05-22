#ifndef BULLET_H
#define BULLET_H

#include "Component.h"
#include "Vec2.h"
#include <string>

class Bullet : public Component {
public:
    // O construtor recebe o ângulo de disparo, a velocidade escalar, o dano, a distância máxima e a imagem
    Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance, std::string sprite);
    
    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) override;
    
    int GetDamage();

private:
    Vec2 speed;         // Velocidade já decomposta em X e Y (baseada no ângulo)
    float distanceLeft; // Quanto a bala ainda pode andar
    int damage;         // Dano que causará ao acertar
};

#endif