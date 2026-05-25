#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Component.h"

class Zombie : public Component {
public:
    Zombie(GameObject& associated);

    void Update(float dt) override;
    void Render() override; // Ficará vazio conforme o PDF

    void Damage(int damage); // Reduz vida

    bool Is(std::string type) override;

    void NotifyCollision(GameObject& other) override;

private:
    int hitpoints;
    float linearSpeed;
};

#endif