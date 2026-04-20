#include "State.h"
#include "SDL2/SDL.h"

State::State() : bg("img/Background.png"), music("audio/BGM.wav") {
    quitRequested = false;
    music.Play(); // Começa a tocar na criação do estado
}

State::~State() {
    // Destrutor vazio por enquanto
}

void State::LoadAssets() {
    // Será usado para carregar recursos futuramente
}

void State::Update(float dt) {
    // Verifica se o utilizador quer sair (Alt+F4 ou clicar no X)
    if (SDL_QuitRequested()) {
        quitRequested = true;
    }
}

void State::Render() {
    bg.Render(0, 0); // Renderiza o fundo na posição (0,0)
}

bool State::QuitRequested() {
    return quitRequested;
}