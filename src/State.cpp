#include "State.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Zombie.h"
#include "TileSet.h"
#include "TileMap.h"
#include "InputManager.h"
#include "Camera.h"
#include "CameraFollower.h"
#include "SDL2/SDL.h"

#ifndef ESCAPE_KEY
    #define ESCAPE_KEY SDLK_ESCAPE
    #define LEFT_MOUSE_BUTTON SDL_BUTTON_LEFT
#endif

// Construtor
State::State() : music("audio/BGM.wav") {
    quitRequested = false;

    // 1. Criar o Background como Objeto
    GameObject* bgObj = new GameObject();
    bgObj->AddComponent(new SpriteRenderer(*bgObj, "img/Background.png"));
    bgObj->box.x = 0;
    bgObj->box.y = 0;
    bgObj->AddComponent(new CameraFollower(*bgObj));
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
    Mix_VolumeMusic(10);
}

State::~State() {
    objectArray.clear(); // unique_ptr limpa a memória automaticamente
}

void State::AddObject(GameObject* go) {
    // Transforma o ponteiro comum em unique_ptr e adiciona ao vetor
    objectArray.emplace_back(go); 
}

void State::Update(float dt) {
    InputManager& input = InputManager::GetInstance();

    // 1. Atualiza a Câmera (faz ela processar o input ou seguir o foco)
    Camera::Update(dt);

    // 2. Verifica se o usuário apertou ESC para sair
    if (input.KeyPress(ESCAPE_KEY) || input.QuitRequested()) {
        quitRequested = true;
    }

    // 3. Exemplo: Se clicar com o mouse, cria um Zombie na posição do mouse
    // IMPORTANTE: O mouse dá a posição na TELA. 
    // Para criar no MUNDO, somamos a posição da Câmera.
    if (input.MousePress(LEFT_MOUSE_BUTTON)) {
        GameObject* go = new GameObject();
        go->box.x = input.GetMouseX() + Camera::pos.x;
        go->box.y = input.GetMouseY() + Camera::pos.y;
        go->AddComponent(new Zombie(*go));
        AddObject(go);
    }

    // 4. Atualiza todos os GameObjects (já passando o dt real)
    for (int i = 0; i < objectArray.size(); i++) {
        objectArray[i]->Update(dt);
    }

    // 5. Lógica de remoção de objetos mortos (opcional neste trabalho)
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