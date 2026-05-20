#ifndef STATE_H
#define STATE_H

#include <vector>
#include <memory> // Obrigatório para shared_ptr e weak_ptr
#include "GameObject.h"
#include "Music.h"

class State {
public:
    State();
    ~State();

    void Start(); // Nova etapa do ciclo de vida

    // Agora retornam e gerenciam ponteiros inteligentes
    std::weak_ptr<GameObject> AddObject(GameObject* go);
    std::weak_ptr<GameObject> GetObjectPtr(GameObject* go);

    void Update(float dt);
    void Render();

    bool QuitRequested();

private:
    void LoadAssets();

    Music music;
    bool quitRequested;
    bool started; // Indica se o Start() já foi executado

    // O vetor agora protege os objetos contra deleção acidental
    std::vector<std::shared_ptr<GameObject>> objectArray;
};

#endif