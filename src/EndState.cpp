#include "EndState.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "InputManager.h"
#include "Game.h"
#include "Camera.h"
#include "Text.h"

EndState::EndState(bool win) : State() {
    // Guarda a variável para usarmos depois
    this->win = win;

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

    // --- TEXTO: Voltar ao Menu ---
    GameObject* textObj = new GameObject();
    
    SDL_Color textColor = win ? SDL_Color{0, 255, 0, 255} : SDL_Color{255, 0, 0, 255};
    
    std::string message = win ? "Pressione ESPACO para jogar novamente!" : "Voce morreu... ESPACO para voltar";
    
    Text* endText = new Text(*textObj, "font/neodgm.ttf", 32, Text::BLENDED, message, textColor);
    textObj->AddComponent(endText);
    
    // Centraliza o texto no ecrã
    textObj->box.x = 600 - (textObj->box.w / 2);
    textObj->box.y = 520;
    
    AddObject(textObj);
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

    if (win) {
        backgroundMusic.Open("audio/endStateWin.ogg");
    } else {
        backgroundMusic.Open("audio/endStateLose.ogg");
    }
    backgroundMusic.Play();
}

void EndState::Pause() {}

void EndState::Resume() {}