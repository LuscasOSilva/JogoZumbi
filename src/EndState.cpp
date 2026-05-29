#include "EndState.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "InputManager.h"
#include "Game.h"
#include "Camera.h"

EndState::EndState(bool win) : State() {
    // 1. Cria o objeto para o fundo da tela
    GameObject* bgObj = new GameObject();

    // 2. Carrega a imagem e a música certas dependendo de vitória ou derrota
    if (win) {
        bgObj->AddComponent(new SpriteRenderer(*bgObj, "img/Win.png"));
        backgroundMusic.Open("audio/endStateWin.ogg");
    } else {
        bgObj->AddComponent(new SpriteRenderer(*bgObj, "img/Lose.png"));
        backgroundMusic.Open("audio/endStateLose.ogg");
    }

    bgObj->box.x = 0;
    bgObj->box.y = 0;
    AddObject(bgObj);
}

EndState::~EndState() {
    // A limpeza do array é feita pelo State pai
}

void EndState::LoadAssets() {
}

void EndState::Update(float dt) {
    InputManager& input = InputManager::GetInstance();

    // Se apertar ESC, sai do jogo
    if (input.KeyPress(SDLK_ESCAPE) || input.QuitRequested()) {
        quitRequested = true;
    }

    // Se apertar ESPAÇO, pede para sair deste estado (volta ao Menu!)
    if (input.KeyPress(SDLK_SPACE)) {
        popRequested = true;
    }

    UpdateArray(dt);
}

void EndState::Render() {
    // Zera a câmera para a imagem ficar centralizada
    Camera::pos = {0, 0};
    RenderArray();
}

void EndState::Start() {
    LoadAssets();
    StartArray();
    backgroundMusic.Play();
}

void EndState::Pause() {}

void EndState::Resume() {}