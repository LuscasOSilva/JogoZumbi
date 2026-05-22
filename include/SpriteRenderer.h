#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "Component.h"
#include "Sprite.h"

class SpriteRenderer : public Component {
public:
    SpriteRenderer(GameObject& associated);
    SpriteRenderer(GameObject& associated, std::string file, int frameCountW = 1, int frameCountH = 1);

    void Update(float dt) override; // Fica vazio
    void Render() override;

    bool Is(std::string type) override;
    
    void Open(std::string file);
    void SetFrame(int frame);
    void SetFlip(SDL_RendererFlip flip);

private:
    Sprite sprite;
};

#endif