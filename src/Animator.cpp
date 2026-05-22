#include "Animator.h"
#include "GameObject.h"
#include "SpriteRenderer.h"

Animator::Animator(GameObject& associated) : Component(associated) {
    frameStart = frameEnd = currentFrame = 0;
    frameTime = timeElapsed = 0;
}

void Animator::Update(float dt) {
    if (frameTime <= 0) return; // Se frameTime for 0, não há animação

    timeElapsed += dt; // Agora somamos frações de segundo reais

    if (timeElapsed >= frameTime) {
        currentFrame++;
        timeElapsed = 0;

        if (currentFrame > frameEnd) {
            currentFrame = frameStart; // Loop da animação
        }

        // Atualiza o frame no SpriteRenderer do mesmo objeto
        SpriteRenderer* sr = (SpriteRenderer*)associated.GetComponent("SpriteRenderer");
        if (sr != nullptr) {
            sr->SetFrame(currentFrame);
        }
    }
}

void Animator::SetAnimation(std::string name) {
    if (this->current != name) {
        this->current = name;
        if (animations.count(name)) {
            Animation anim = animations[name];
            frameStart = anim.frameStart;
            frameEnd = anim.frameEnd;
            frameTime = anim.frameTime;
            currentFrame = frameStart;
            timeElapsed = 0;

            SpriteRenderer* sr = (SpriteRenderer*)associated.GetComponent("SpriteRenderer");
            if (sr != nullptr) sr->SetFrame(currentFrame);
            //if (sr != nullptr) sr->SetFrame(currentFrame, anim.flip);
        }
    }
}

void Animator::AddAnimation(std::string name, Animation anim) {
    if (animations.find(name) == animations.end()) {
        animations.emplace(name, anim);
    }
}

void Animator::Render() {}

bool Animator::Is(std::string type) { return type == "Animator"; }