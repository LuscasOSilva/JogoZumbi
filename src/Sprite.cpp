#include "Sprite.h"
#include "Game.h"
#include "Resources.h"
#include "SDL2/SDL_image.h"
#include <iostream>

Sprite::Sprite() {
    texture = nullptr;
    scale = Vec2(1, 1); // Escala original (100%)
    frameCountW = 1;
    frameCountH = 1;
    currentFrame = 0;
}

Sprite::Sprite(std::string file) {
    texture = nullptr;
    scale = Vec2(1, 1); // Escala original (100%)
    frameCountW = 1;
    frameCountH = 1;
    currentFrame = 0;
    Open(file);
}

Sprite::~Sprite() {
    // Resources já cuida disso
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

int Sprite::GetWidth() { return (width / frameCountW) * scale.x; }
int Sprite::GetHeight() { return (height / frameCountH) * scale.y; }
bool Sprite::IsOpen() { return texture != nullptr; }

void Sprite::Render(int x, int y, float angle) {
    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;
    // A largura e altura reais são multiplicadas pela escala
    dstRect.w = clipRect.w * scale.x; 
    dstRect.h = clipRect.h * scale.y;

    // SDL_RenderCopyEx permite rotação e espelhamento
    SDL_RenderCopyEx(
        Game::GetInstance().GetRenderer(),
        texture,
        &clipRect,
        &dstRect,
        angle, // Lê o ângulo atual do GameObject
        nullptr,             // nullptr faz rodar em torno do centro do próprio dstRect
        flip        // Por enquanto sem espelhamento, trataremos disto na arma
    );
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

void Sprite::SetScale(float scaleX, float scaleY) {
    // Garante que a escala não seja zero para evitar erros de renderização
    scale.x = scaleX != 0 ? scaleX : scale.x;
    scale.y = scaleY != 0 ? scaleY : scale.y;
}

Vec2 Sprite::GetScale() {
    return scale;
}

void Sprite::SetFlip(SDL_RendererFlip flip) {
    this->flip = flip;
}