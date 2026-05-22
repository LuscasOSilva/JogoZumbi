#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Component.h"
#include "Animation.h"
#include <unordered_map>
#include <string>

class Animator : public Component {
public:
    Animator(GameObject& associated);
    void Update(float dt) override;
    void Render() override; // Fica vazia

    void SetAnimation(std::string name);
    void AddAnimation(std::string name, Animation anim);

    bool Is(std::string type) override;

    std::string current;

private:
    std::unordered_map<std::string, Animation> animations;
    int frameStart, frameEnd, currentFrame;
    float frameTime, timeElapsed;
};

#endif