#include "Sprite.h"
#include "Game.h"
#include "SDL2/SDL_image.h"
#include <iostream>

Sprite::Sprite() {
    texture = nullptr; // Inicializa como nulo
}

Sprite::Sprite(std::string file) {
    texture = nullptr;
    Open(file);        // Chama o Open diretamente
}

Sprite::~Sprite() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture); // Libera a memória da textura
    }
}

void Sprite::Open(std::string file) {
    // Se já houver uma textura, libere-a antes de carregar outra
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }

    // Carrega a textura usando o renderer do Singleton Game
    texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());

    if (texture == nullptr) {
        std::cerr << "Erro ao carregar textura: " << SDL_GetError() << std::endl;
        return;
    }

    // Obtém as dimensões da imagem
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

    // Define o clip inicial com o tamanho total da imagem
    SetClip(0, 0, width, height);
}

void Sprite::SetClip(int x, int y, int w, int h) {
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = w;
    clipRect.h = h;
}

int Sprite::GetWidth() { return width; }
int Sprite::GetHeight() { return height; }
bool Sprite::IsOpen() { return texture != nullptr; }

void Sprite::Render(int x, int y) {
    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = clipRect.w;
    dstRect.h = clipRect.h;

    // Renderiza a parte selecionada (clipRect) na posição de destino (dstRect)
    SDL_RenderCopy(Game::GetInstance().GetRenderer(), texture, &clipRect, &dstRect);
}