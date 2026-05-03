#include "State.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Zombie.h"
#include "TileSet.h"
#include "TileMap.h"
#include "SDL2/SDL.h"

State::State() : music("audio/BGM.wav") {
    quitRequested = false;

    // 1. Criar o Background como Objeto
    GameObject* bgObj = new GameObject();
    bgObj->AddComponent(new SpriteRenderer(*bgObj, "img/Background.png"));
    bgObj->box.x = 0;
    bgObj->box.y = 0;
    AddObject(bgObj);

    GameObject* mapObj = new GameObject();
    mapObj->box.x = 0;
    mapObj->box.y = 0;

    // Cria o TileSet e o TileMap (dimensões 64x64 conforme o PDF)
    TileSet* ts = new TileSet(64, 64, "img/Tileset.png");
    TileMap* tm = new TileMap(*mapObj, "map/map.txt", ts);

    mapObj->AddComponent(tm);
    AddObject(mapObj); // Adicionamos o mapa ANTES dos inimigos para ele ficar no fundo

    // 2. Criar o Zombie (O que vai mudar o visual!)
    GameObject* zombieObj = new GameObject();
    zombieObj->box.x = 600; // Posição central 
    zombieObj->box.y = 450; 
    zombieObj->AddComponent(new Zombie(*zombieObj));
    
    AddObject(zombieObj);

    music.Play();
}

State::~State() {
    objectArray.clear(); // unique_ptr limpa a memória automaticamente
}

void State::AddObject(GameObject* go) {
    // Transforma o ponteiro comum em unique_ptr e adiciona ao vetor
    objectArray.emplace_back(go); 
}

void State::Update(float dt) {
    // 1. Verifica fechamento da janela
    if (SDL_QuitRequested()) {
        quitRequested = true;
    }

    // 2. Atualiza todos os objetos
    for (size_t i = 0; i < objectArray.size(); i++) {
        objectArray[i]->Update(dt);
    }

    // 3. Remove objetos mortos 
    for (size_t i = 0; i < objectArray.size(); i++) {
        if (objectArray[i]->IsDead()) {
            // Usa erase com o iterador de início + índice
            objectArray.erase(objectArray.begin() + i);
            i--; // Ajusta o índice após a remoção
        }
    }
}

void State::Render() {
    // Renderiza todos os objetos do array
    for (auto& go : objectArray) {
        go->Render();
    }
}

bool State::QuitRequested() {
    return quitRequested;
}