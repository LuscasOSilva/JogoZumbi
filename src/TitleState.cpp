#include "TitleState.h"
#include "StageState.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "InputManager.h"
#include "Game.h"
#include "Camera.h"

TitleState::TitleState() : State() {
    // 1. Criar o objeto de Background da tela de título
    GameObject* bgObj = new GameObject();
    bgObj->AddComponent(new SpriteRenderer(*bgObj, "img/Title.png"));
    bgObj->box.x = 0;
    bgObj->box.y = 0;
    
    // Adiciona o background ao vetor de objetos deste estado
    AddObject(bgObj);
}

TitleState::~TitleState() {
}

void TitleState::LoadAssets() {
    // Recursos específicos do menu podem ser carregados/pré-carregados aqui
}

void TitleState::Update(float dt) {
    InputManager& input = InputManager::GetInstance();

    // Se apertar ESC ou fechar a janela, pede para fechar o jogo inteiro
    if (input.KeyPress(SDLK_ESCAPE) || input.QuitRequested()) {
        quitRequested = true;
    }

    // Se apertar ESPAÇO, inicia a fase de gameplay empilhando o StageState!
    if (input.KeyPress(SDLK_SPACE)) {
        Game::GetInstance().Push(new StageState());
    }

    // Atualiza os GameObjects deste estado (o background, textos, etc.)
    UpdateArray(dt);
}

void TitleState::Render() {
    // Como a câmera pode ter se movido na fase anterior, resetamos a posição da câmera no menu
    Camera::pos = {0, 0};

    // Renderiza os objetos deste estado
    RenderArray();
}

void TitleState::Start() {
    LoadAssets();
    StartArray();
}

void TitleState::Pause() {
    // Vazio por enquanto (chamado quando o StageState for empilhado por cima deste menu)
}

void TitleState::Resume() {
    // Vazio por enquanto (chamado caso o jogo volte para esta tela)
}