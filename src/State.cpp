#include "State.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Zombie.h"
#include "TileSet.h"
#include "TileMap.h"
#include "InputManager.h"
#include "Camera.h"
#include "CameraFollower.h"
#include "Character.h"
#include "PlayerController.h"
#include "Collision.h"
#include "Collider.h"
#include "Spawner.h"
#include "SDL2/SDL.h"

#ifndef ESCAPE_KEY
    #define ESCAPE_KEY SDLK_ESCAPE
    #define LEFT_MOUSE_BUTTON SDL_BUTTON_LEFT
#endif

// Construtor unificado
State::State() : music("audio/BGM.wav") {
    quitRequested = false;
    started = false;

    // 1. Criar o Background
    GameObject* bgObj = new GameObject();
    bgObj->AddComponent(new SpriteRenderer(*bgObj, "img/Background.png"));
    bgObj->box.x = 0;
    bgObj->box.y = 0;
    bgObj->AddComponent(new CameraFollower(*bgObj));
    AddObject(bgObj);

    // 2. Criar o Mapa
    GameObject* mapObj = new GameObject();
    mapObj->box.x = 0;
    mapObj->box.y = 0;
    TileSet* ts = new TileSet(64, 64, "img/Tileset.png");
    TileMap* tm = new TileMap(*mapObj, "map/map.txt", ts);
    mapObj->AddComponent(tm);
    AddObject(mapObj);

    // 3. Instanciar o nosso Jogador
    GameObject* playerGo = new GameObject();
    playerGo->box.x = 1280; // Posição central do mapa recomendada pelo PDF
    playerGo->box.y = 1280;

    // Adiciona o corpo e o cérebro
    playerGo->AddComponent(new Character(*playerGo, "img/Player.png"));
    playerGo->AddComponent(new PlayerController(*playerGo));

    AddObject(playerGo);

    // Opcional: Fazer a câmera seguir o jogador automaticamente
    Camera::Follow(playerGo);

    // 4. Criar o Zombie
    GameObject* zombieObj = new GameObject();
    zombieObj->box.x = 600; 
    zombieObj->box.y = 450; 
    zombieObj->AddComponent(new Zombie(*zombieObj));
    AddObject(zombieObj);

    // 5. Instanciar o Spawner (Gerador de Inimigos)
    GameObject* spawnerGo = new GameObject();
    // Cria um zumbi a cada 3.5 segundos
    spawnerGo->AddComponent(new Spawner(*spawnerGo, 3.5f)); 
    AddObject(spawnerGo);

    music.Play();
    Mix_VolumeMusic(10);
}

State::~State() {
    objectArray.clear(); 
}

void State::Start() {
    // Passa por todos os objetos criados no construtor e inicializa-os
    for (size_t i = 0; i < objectArray.size(); i++) {
        objectArray[i]->Start();
    }
    started = true; // A partir de agora, novos objetos dão Start imediatamente
}


std::weak_ptr<GameObject> State::AddObject(GameObject* go) {
    // Transforma o ponteiro bruto em um ponteiro inteligente gerenciado
    std::shared_ptr<GameObject> sh_go(go);
    objectArray.push_back(sh_go);

    // Se a fase já começou, o objeto recém-criado precisa do seu Start()
    if (started) {
        sh_go->Start();
    }

    // Retorna uma referência fraca (segura para os outros objetos monitorarem)
    return std::weak_ptr<GameObject>(sh_go);
}

std::weak_ptr<GameObject> State::GetObjectPtr(GameObject* go) {
    // Procura o objeto no vetor para retornar o weak_ptr correspondente
    for (size_t i = 0; i < objectArray.size(); i++) {
        if (objectArray[i].get() == go) {
            return std::weak_ptr<GameObject>(objectArray[i]);
        }
    }
    return std::weak_ptr<GameObject>(); // Retorna vazio se não achar
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
    /*if (input.MousePress(LEFT_MOUSE_BUTTON)) {
        GameObject* go = new GameObject();
        go->box.x = input.GetMouseX() + Camera::pos.x;
        go->box.y = input.GetMouseY() + Camera::pos.y;
        go->AddComponent(new Zombie(*go));
        AddObject(go);
    }*/

    // 4. Atualiza todos os GameObjects (já passando o dt real)
    for (size_t i = 0; i < objectArray.size(); i++) {
        objectArray[i]->Update(dt);
    }

    // --- VERIFICAÇÃO DE COLISÕES ---
    for (size_t i = 0; i < objectArray.size(); i++) {
        for (size_t j = i + 1; j < objectArray.size(); j++) {
            
            // Tenta pegar o componente Collider de ambos os objetos
            Collider* a = (Collider*)objectArray[i]->GetComponent("Collider");
            Collider* b = (Collider*)objectArray[j]->GetComponent("Collider");

            // Se os dois objetos possuem Collider, testamos a batida
            if (a != nullptr && b != nullptr) {
                // O IsColliding recebe os Rects e os ângulos em radianos
                if (Collision::IsColliding(a->box, b->box, objectArray[i]->angleDeg * (M_PI / 180.0), objectArray[j]->angleDeg * (M_PI / 180.0))) {
                    
                    // Se colidiu, avisa os dois objetos!
                    objectArray[i]->NotifyCollision(*objectArray[j]);
                    objectArray[j]->NotifyCollision(*objectArray[i]);
                }
            }
        }
    }
    // --- LIMPEZA DE OBJETOS MORTOS ---
    // Varremos o array de trás para a frente. Assim, ao apagar um elemento, 
    // não estragamos a ordem dos índices dos elementos que ainda não verificámos.
    for (int i = objectArray.size() - 1; i >= 0; i--) {
        if (objectArray[i]->IsDead()) {
            objectArray.erase(objectArray.begin() + i);
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