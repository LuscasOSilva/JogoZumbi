#include "Zombie.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Sound.h"
#include "Collider.h"
#include "Bullet.h"
#include "Character.h"
#include "Vec2.h"
#include <iostream>

Zombie::Zombie(GameObject& associated) : Component(associated) {
    hitpoints = 100; // Sugestão do PDF
    linearSpeed = 120.0f;

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

    // Imprime no terminal a vida do zumbi sempre que ele leva um tiro
    //std::cout << "Zumbi atingido! HP restante: " << hitpoints << std::endl;

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

    // 1. Se o zumbi estiver morto, ele não se mexe
    if (hitpoints <= 0) {
        return;
    }

    // 2. Verifica se o jogador existe no mundo
    if (Character::player != nullptr) {
        
        // Pega o centro do jogador e o centro do próprio zumbi
        Vec2 playerCenter = Character::player->GetBox().GetCenter();
        Vec2 zombieCenter = associated.box.GetCenter();

        // Calcula o vetor de direção do zumbi até o jogador
        Vec2 direction = playerCenter - zombieCenter;

        // Se o zumbi não estiver exatamente em cima do jogador, ele move-se
        if (direction.GetMagnitude() > 0) {
            // Normaliza o vetor (faz o tamanho ser 1) para manter a velocidade constante
            Vec2 normalizedDir = direction.GetNormalized();

            // Deslocamento = direção * velocidade escalar * tempo
            Vec2 displacement = normalizedDir * linearSpeed * dt;

            // Move a caixa de colisão e o sprite do zumbi no mapa
            associated.box.x += displacement.x;
            associated.box.y += displacement.y;

            // Opcional: Atualiza o ângulo do zumbi para ele olhar na direção que caminha
            // associated.angleDeg = normalizedDir.GetAngle() * (180.0 / M_PI);
            
            // POLIMENTO EXTRA: Espelha o sprite do zumbi se ele andar para a esquerda
            SpriteRenderer* sr = (SpriteRenderer*)associated.GetComponent("SpriteRenderer");
            if (sr != nullptr) {
                if (displacement.x < 0) sr->SetFlip(SDL_FLIP_HORIZONTAL);
                else if (displacement.x > 0) sr->SetFlip(SDL_FLIP_NONE);
            }
        }
    }
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