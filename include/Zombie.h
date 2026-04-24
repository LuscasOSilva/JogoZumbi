#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Component.h"

class Zombie : public Component {
public:
    Zombie(GameObject& associated);

    void Update(float dt) override;
    void Render() override; // Ficará vazio conforme o PDF

    void Damage(int damage); // Reduz vida

private:
    int hitpoints;
};

#endif