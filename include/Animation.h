#ifndef ANIMATION_H
#define ANIMATION_H

class Animation {
public:
    int frameStart;
    int frameEnd;
    float frameTime;

    // Construtor padrão (adicionado para o unordered_map funcionar)
    Animation() : frameStart(0), frameEnd(0), frameTime(0) {}

    Animation(int frameStart, int frameEnd, float frameTime) 
        : frameStart(frameStart), frameEnd(frameEnd), frameTime(frameTime) {}
};

#endif