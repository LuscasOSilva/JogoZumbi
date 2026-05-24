#define DEBUG

#include "Collider.h"
#include "GameObject.h"
#include "Camera.h"
#include "Game.h"
#include <cmath>

// Define a constante de PI, caso não exista
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Collider::Collider(GameObject& associated, Vec2 scale, Vec2 offset) 
    : Component(associated), scale(scale), offset(offset) {
}

void Collider::Update(float dt) {
    // 1. Copia a largura e altura do associated, aplicando a escala
    box.w = associated.box.w * scale.x;
    box.h = associated.box.h * scale.y;

    // 2. Transforma o ângulo do GameObject de graus para radianos
    float angleRad = associated.angleDeg * (M_PI / 180.0);
    
    // 3. Roda o offset de acordo com o ângulo atual do GameObject
    Vec2 offsetRotated = offset;
    offsetRotated.Rotate(angleRad);

    // 4. O novo centro é o centro do objeto original mais o offset rotacionado
    Vec2 center = associated.box.GetCenter() + offsetRotated;
    
    // 5. Reposiciona o x e y reais da box baseando-se no novo centro
    box.x = center.x - (box.w / 2.0);
    box.y = center.y - (box.h / 2.0);
}

void Collider::Render() {
#ifdef DEBUG
    Vec2 center( box.GetCenter() );
    SDL_Point points[5];

    // Transcrição exata da função de Debug fornecida
    // Ponto 0 e 4
    Vec2 point = Vec2(box.x, box.y) - center;
    point.Rotate( associated.angleDeg / (180.0 / M_PI) );
    point = point + center - Camera::pos;
    points[0] = {(int)point.x, (int)point.y};
    points[4] = {(int)point.x, (int)point.y};
    
    // Ponto 1
    point = Vec2(box.x + box.w, box.y) - center;
    point.Rotate( associated.angleDeg / (180.0 / M_PI) );
    point = point + center - Camera::pos;
    points[1] = {(int)point.x, (int)point.y};
    
    // Ponto 2
    point = Vec2(box.x + box.w, box.y + box.h) - center;
    point.Rotate( associated.angleDeg / (180.0 / M_PI) );
    point = point + center - Camera::pos;
    points[2] = {(int)point.x, (int)point.y};
    
    // Ponto 3
    point = Vec2(box.x, box.y + box.h) - center;
    point.Rotate( associated.angleDeg / (180.0 / M_PI) );
    point = point + center - Camera::pos;
    points[3] = {(int)point.x, (int)point.y};

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif // DEBUG
}

void Collider::SetScale(Vec2 scale) {
    this->scale = scale;
}

void Collider::SetOffset(Vec2 offset) {
    this->offset = offset;
}

bool Collider::Is(std::string type) {
    return type == "Collider";
}