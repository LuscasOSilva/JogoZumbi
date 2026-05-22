#include "PlayerController.h"
#include "InputManager.h"
#include "Character.h"
#include "Camera.h"
#include "GameObject.h"

#ifndef SDL_BUTTON_LEFT
#define SDL_BUTTON_LEFT 1
#endif

PlayerController::PlayerController(GameObject& associated) : Component(associated) {}

void PlayerController::Start() {}

void PlayerController::Update(float dt) {
    InputManager& input = InputManager::GetInstance();
    
    // Tenta pegar o componente Character que está colado neste mesmo GameObject
    Character* charComp = (Character*)associated.GetComponent("Character");
    if (!charComp) return;

    // 1. Movimentação (W, A, S, D)
    Vec2 direction(0, 0);
    if (input.IsKeyDown(SDLK_w)) direction.y -= 1;
    if (input.IsKeyDown(SDLK_s)) direction.y += 1;
    if (input.IsKeyDown(SDLK_a)) direction.x -= 1;
    if (input.IsKeyDown(SDLK_d)) direction.x += 1;

    // Se pressionou alguma tecla, emite a ordem de movimento
    if (direction.x != 0 || direction.y != 0) {
        // Normaliza para que andar na diagonal não seja mais rápido
        direction = direction.GetNormalized();
        charComp->Issue(Character::Command(Character::MOVE, direction.x, direction.y));
    } else {
        // Envia comando de parar (vetor zero)
        charComp->Issue(Character::Command(Character::MOVE, 0, 0));
    }

    // 2. Tiro (Clique do Mouse)
    if (input.MousePress(SDL_BUTTON_LEFT)) {
        // Pega a posição do mouse na TELA e converte para MUNDO somando a Câmera
        float mouseX = input.GetMouseX() + Camera::pos.x;
        float mouseY = input.GetMouseY() + Camera::pos.y;
        
        charComp->Issue(Character::Command(Character::SHOOT, mouseX, mouseY));
    }
}

void PlayerController::Render() {}

bool PlayerController::Is(std::string type) {
    return type == "PlayerController";
}