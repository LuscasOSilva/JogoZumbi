#include "Zombie.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Sound.h"
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

    associated.AddComponent(new Sound(associated, "audio/dead.wav"));
}

void Zombie::Damage(int damage) {
    // Reduz hitpoints
    hitpoints -= damage;
    
    // Se a vida chegar a zero, troca para a animação de morto
    if (hitpoints <= 0) {
        Animator* anim = (Animator*)associated.GetComponent<Animator>();
        if (anim != nullptr) {
            anim->SetAnimation("dead");
        }
        
        // 2. Toca o som de morte
        // Buscamos o componente Sound associado a este GameObject
        Sound* sound = (Sound*)associated.GetComponent<Sound>();
        if (sound != nullptr) {
            sound->Play(1); // Play(1) toca o som exatamente uma vez[cite: 2]
        }
    }
}

void Zombie::Update(float dt) {
    // Por enquanto, causa 1 de dano por frame para testar a morte
    if (hitpoints > 0) {
        Damage(1);
    }
}

void Zombie::Render() {
    // Vazio, pois o SpriteRenderer já cuida do desenho
}