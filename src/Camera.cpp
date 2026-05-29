#include "Camera.h"
#include "InputManager.h"
#include "Game.h"

// Inicialização de membros estáticos
Vec2 Camera::pos = {0, 0};
Vec2 Camera::speed = {400, 400}; // Velocidade de movimento manual
GameObject* Camera::focus = nullptr;

void Camera::Follow(GameObject* newFocus) { focus = newFocus; }
void Camera::Unfollow() { focus = nullptr; }

void Camera::Update(float dt) {
    if (focus != nullptr) {
        // Centraliza a câmera no objeto (ajustando para o meio da tela)
        // Considerando tela de 1024x600 (ajuste se a sua for diferente)
        Camera::pos.x = focus->box.GetCenter().x - 600;
        Camera::pos.y = focus->box.GetCenter().y - 450;
    } else {
        // Movimento manual via teclado (Setas ou WASD)
        InputManager& input = InputManager::GetInstance();
        if (input.IsKeyDown(SDLK_UP))    pos.y -= speed.y * dt;
        if (input.IsKeyDown(SDLK_DOWN))  pos.y += speed.y * dt;
        if (input.IsKeyDown(SDLK_LEFT))  pos.x -= speed.x * dt;
        if (input.IsKeyDown(SDLK_RIGHT)) pos.x += speed.x * dt;
    }
}