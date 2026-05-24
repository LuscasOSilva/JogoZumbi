#include "Zombie.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Sound.h"
#include "Collider.h"
#include "Bullet.h"
#include <iostream>

Zombie::Zombie(GameObject& associated) : Component(associated) {
    hitpoints = 100; // Sugestão do PDF 

    // Cria o SpriteRenderer para o Zombie (3x2 frames)
    SpriteRenderer* sr = new SpriteRenderer(associated, "img/Enemy.png", 3, 2);
    associated.AddComponent(sr);

    Animator* anim = new Animator(associated);

    anim->AddAnimation("walking", Animation(0, 3, 10));
    anim->AddAnimation("dead", Animation(5, 5, 0));

    associated.AddComponent(anim);

    anim->SetAnimation("walking"); // Começa andando1

    associated.AddComponent(new Sound(associated, "audio/Dead.wav"));

    // Adiciona a caixa de colisão ao inimigo
    associated.AddComponent(new Collider(associated));
}

void Zombie::Damage(int damage) {
    hitpoints -= damage;
    
    if (hitpoints <= 0) {
        Animator* anim = (Animator*)associated.GetComponent("Animator");
        if (anim != nullptr) {
            anim->SetAnimation("dead");
        }

        Sound* sound = (Sound*)associated.GetComponent("Sound");
        if (sound != nullptr) {
            sound->Play(1); 
        }

        // NOVO POLIMENTO DO TRABALHO 6: Remove o Collider do Zumbi morto!
        Collider* collider = (Collider*)associated.GetComponent("Collider");
        if (collider != nullptr) {
            associated.RemoveComponent(collider);
        }
    }
}

void Zombie::Update(float dt) {
    // Por enquanto, causa 1 de dano por frame para testar a morte
    // if (hitpoints > 0) Damage(1);
}

void Zombie::Render() {
    // Vazio, pois o SpriteRenderer já cuida do desenho
}

bool Zombie::Is(std::string type) { return type == "Zombie"; }

void Zombie::NotifyCollision(GameObject& other) {
    // Verifica se o que bateu nele foi uma bala
    Bullet* bullet = (Bullet*)other.GetComponent("Bullet");
    
    if (bullet != nullptr) {
        // Recebe o dano da bala
        Damage(bullet->GetDamage());
    }
}