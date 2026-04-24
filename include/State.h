#ifndef STATE_H
#define STATE_H

#include <vector>
#include <memory>
#include "GameObject.h"
#include "Music.h"

class State {
public:
    State();
    ~State();

    bool QuitRequested();
    void Update(float dt);
    void Render();
    void AddObject(GameObject* go); // Adiciona novos objetos [cite: 431]

private:
    Music music;
    bool quitRequested;
    // Vetor de ponteiros inteligentes para GameObjects [cite: 432]
    std::vector<std::unique_ptr<GameObject>> objectArray; 
};

#endif