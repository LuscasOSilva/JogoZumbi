#include "Sprite.h"
#include "Game.h"
#include "Resources.h"
#include "SDL2/SDL_image.h"
#include <iostream>

Sprite::Sprite() {
    texture = nullptr;
    frameCountW = 1;
    frameCountH = 1;
    currentFrame = 0;
}

Sprite::Sprite(std::string file) {
    texture = nullptr;
    frameCountW = 1;
    frameCountH = 1;
    currentFrame = 0;
    Open(file);
}

Sprite::~Sprite() {
    if (texture != nullptr) {
        //SDL_DestroyTexture(texture); // Libera a memória da textura
    }
}

void Sprite::Open(std::string file) {
    // Carrega a textura usando o renderer do Singleton Game
    texture = Resources::GetImage(file);

    if (texture == nullptr) {
        std::cerr << "Erro ao carregar textura: " << SDL_GetError() << std::endl;
        return;
    }

    // Obtém as dimensões da imagem
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

    // Define o clip inicial com o tamanho total da imagem
    SetFrame(0);
}

void Sprite::SetClip(int x, int y, int w, int h) {
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = w;
    clipRect.h = h;
}

int Sprite::GetWidth() { return (width / frameCountW); }
int Sprite::GetHeight() { return (height / frameCountH); }
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

void Sprite::SetFrame(int frame) {
    currentFrame = frame;
    // Calcula a largura e altura de UM frame
    int frameWidth = width / frameCountW;
    int frameHeight = height / frameCountH;

    // Lógica do PDF: divisão dá a linha, resto dá a coluna
    int column = frame % frameCountW;
    int row = frame / frameCountW;

    SetClip(column * frameWidth, row * frameHeight, frameWidth, frameHeight);
}

void Sprite::SetFrameCount(int frameCountW, int frameCountH) {
    this->frameCountW = frameCountW;
    this->frameCountH = frameCountH;
    // O PDF sugere que, ao mudar a contagem de frames, 
    // devemos atualizar o clip para o frame 0
    SetFrame(0);
}