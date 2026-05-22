#include "Gun.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Bullet.h"
#include "Game.h"
#include "GameObject.h"
#include "State.h"
#include <cmath>

// Define o M_PI se não existir no sistema
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Gun::Gun(GameObject& associated, std::weak_ptr<GameObject> character) 
    : Component(associated), character(character) {
    
    // 1. Adiciona os componentes visuais
    // Avisa que a arma tem 3 colunas e 2 linhas
    associated.AddComponent(new SpriteRenderer(associated, "img/Gun.png", 3, 2));
    
    Animator* anim = new Animator(associated);
    
    // A arma tem animações de ficar parada e recarregando
    // anim->AddAnimation("idle", 0, 0, 1.0f);
    // anim->AddAnimation("reloading", 1, 5, 0.1f); 
    
    associated.AddComponent(anim);
    
    // 2. Cria os sons (não precisamos adicioná-los à box do GameObject, podemos apenas instanciá-los)
    shotSound = new Sound(associated, "audio/Range.wav");
    reloadSound = new Sound(associated, "audio/PumpAction.mp3");

    // 3. Inicializa variáveis de controle
    cooldownState = 0; // 0 = Pronta para atirar
    angle = 0;
}

void Gun::Update(float dt) {
    // 1. Verifica se o dono (Character) ainda existe. Se morreu, a arma também morre.
    std::shared_ptr<GameObject> charPtr = character.lock();
    if (!charPtr) {
        associated.RequestDelete();
        return;
    }

    // 2. Segue o personagem: O centro da arma fica no centro do personagem
    Vec2 charCenter = charPtr->box.GetCenter();
    associated.box.x = charCenter.x - (associated.box.w / 2.0);
    associated.box.y = charCenter.y - (associated.box.h / 2.0);

    // Reposiciona a arma a uma certa distância (ex: 40 pixels) na direção que ela está a apontar
    float distance = 40.0f;
    associated.box.x += distance * std::cos(angle);
    associated.box.y += distance * std::sin(angle);

    // 3. Máquina de Estados da Recarga (Cooldown)
    if (cooldownState != 0) {
        cdTimer.Update(dt);
        Animator* anim = (Animator*)associated.GetComponent("Animator");

        // Estado 1: Tempo entre o tiro e o início da recarga (ex: 0.3s)
        if (cooldownState == 1 && cdTimer.Get() > 0.3f) {
            cooldownState = 2;
            cdTimer.Restart();
            if (anim) anim->SetAnimation("reloading");
            reloadSound->Play();
        }
        // Estado 2: Tempo da animação de recarga (ex: 0.5s)
        else if (cooldownState == 2 && cdTimer.Get() > 0.5f) {
            cooldownState = 3;
            cdTimer.Restart();
            if (anim) anim->SetAnimation("idle");
        }
        // Estado 3: Tempo final até a arma estar pronta (ex: 0.2s)
        else if (cooldownState == 3 && cdTimer.Get() > 0.2f) {
            cooldownState = 0; // Pronta para atirar novamente!
            cdTimer.Restart();
        }
    }
}

void Gun::Shoot(Vec2 target) {
    if (cooldownState == 0) {
        // 1. Calcula o ângulo em direção ao clique do rato
        Vec2 gunCenter = associated.box.GetCenter();
        angle = gunCenter.GetAngle(target);
        associated.angleDeg = angle * 180.0 / M_PI;

        // 2. Toca o som do tiro
        shotSound->Play();

        // 3. Cria a bala
        GameObject* bulletGo = new GameObject();
        
        // A bala nasce na ponta da arma
        float gunBarrelLength = 30.0f; 
        bulletGo->box.x = gunCenter.x + gunBarrelLength * std::cos(angle);
        bulletGo->box.y = gunCenter.y + gunBarrelLength * std::sin(angle);

        // Instancia a bala (ângulo, velocidade, dano, max distance, sprite)
        bulletGo->AddComponent(new Bullet(*bulletGo, angle, 400.0f, 10, 800.0f, "img/Bullet.png"));
        
        Game::GetInstance().GetState().AddObject(bulletGo);

        // 4. Inicia o cooldown
        cooldownState = 1;
        cdTimer.Restart();
    }
}

void Gun::Render() {
    // Vazio, a renderização já é tratada pelo SpriteRenderer
}

bool Gun::Is(std::string type) {
    return type == "Gun";
}