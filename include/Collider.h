#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"
#include "Vec2.h"
#include "Rect.h"
#include <string>

class Collider : public Component {
public:
    // Construtor com valores padrão para scale e offset
    Collider(GameObject& associated, Vec2 scale = {1, 1}, Vec2 offset = {0, 0});

    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) override;

    // Funções para atualizar a escala e o deslocamento
    void SetScale(Vec2 scale);
    void SetOffset(Vec2 offset);

    // A caixa de colisão real
    Rect box;

private:
    Vec2 scale;
    Vec2 offset;
};

#endif