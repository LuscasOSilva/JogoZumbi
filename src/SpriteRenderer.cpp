#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Camera.h"

SpriteRenderer::SpriteRenderer(GameObject& associated) : Component(associated) {}

SpriteRenderer::SpriteRenderer(GameObject& associated, std::string file, int frameCountW, int frameCountH) 
    : Component(associated) {
    sprite.SetFrameCount(frameCountW, frameCountH);
    Open(file);
}

void SpriteRenderer::Open(std::string file) {
    sprite.Open(file);
    // Ajusta o tamanho da box do GameObject para o tamanho do sprite
    associated.box.w = sprite.GetWidth();
    associated.box.h = sprite.GetHeight();
}

void SpriteRenderer::Render() {
    // Renderiza na posição da box do GameObject
    sprite.Render(associated.box.x - Camera::pos.x, associated.box.y - Camera::pos.y, associated.angleDeg);
}

void SpriteRenderer::Update(float dt) {}

void SpriteRenderer::SetFrame(int frame) { sprite.SetFrame(frame); }

// Adicionado o Is() obrigatório!
bool SpriteRenderer::Is(std::string type) {
    return type == "SpriteRenderer";
}

void SpriteRenderer::SetFlip(SDL_RendererFlip flip) {
    sprite.SetFlip(flip);
}