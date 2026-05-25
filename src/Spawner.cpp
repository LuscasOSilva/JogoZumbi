#include "Spawner.h"
#include "GameObject.h"
#include "Zombie.h"
#include "Game.h"
#include "State.h"
#include <cstdlib> // Necessário para usar a função rand()

Spawner::Spawner(GameObject& associated, float cooldownTime) 
    : Component(associated), cooldownTime(cooldownTime) {
}

void Spawner::Update(float dt) {
    timer.Update(dt);

    if (timer.Get() > cooldownTime) {
        // Reinicia o cronômetro para o próximo respawn
        timer.Restart();

        // 1. Cria o GameObject do novo Zumbi
        GameObject* zombieGo = new GameObject();

        // 2. Define uma posição aleatória no mapa
        // O mapa tem cerca de 2560x2560 pixels (40 tiles de 64px)
        zombieGo->box.x = rand() % 2000 + 100; // Valor entre 100 e 2100
        zombieGo->box.y = rand() % 2000 + 100;

        // 3. Adiciona o componente Zombie (que já inclui o Sprite, Collider, etc.)
        zombieGo->AddComponent(new Zombie(*zombieGo));

        // 4. Adiciona o novo Zumbi ao State do jogo
        Game::GetInstance().GetState().AddObject(zombieGo);
    }
}

void Spawner::Render() {
    // O Spawner é um componente invisível, não desenha nada
}

bool Spawner::Is(std::string type) {
    return type == "Spawner";
}