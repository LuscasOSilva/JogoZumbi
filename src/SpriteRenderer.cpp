#include "SpriteRenderer.h"
#include "GameObject.h"

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
    sprite.Render(associated.box.x, associated.box.y);
}

void SpriteRenderer::Update(float dt) {}

void SpriteRenderer::SetFrame(int frame) { sprite.SetFrame(frame); }