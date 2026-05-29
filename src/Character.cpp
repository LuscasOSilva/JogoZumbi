#include "Character.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Game.h"
#include "Gun.h"
#include "StageState.h"
#include "GameObject.h"
#include "Collider.h"
#include "Zombie.h"
#include "Camera.h"

// Inicialização do ponteiro estático global
Character* Character::player = nullptr;

Character::Character(GameObject& associated, std::string sprite) : Component(associated) {
    // 1. O Character atual torna-se o player global
    player = this;
    
    // 2. Inicializa atributos
    hp = 100;
    linearSpeed = 200; // Pixels por segundo
    speed = Vec2(0, 0);

    // 3. Adiciona os componentes visuais
    // Avisa que a imagem tem 3 colunas e 4 linhas
    associated.AddComponent(new SpriteRenderer(associated, sprite, 3, 4));
    
    // Cria o Animator e guarda num ponteiro para configurarmos as animações
    Animator* anim = new Animator(associated);
    
    // Adiciona as animações passando o (frameInicial, frameFinal, tempoPorFrame)
    anim->AddAnimation("idle", Animation(0, 0, 1.0f));
    anim->AddAnimation("walking", Animation(0, 5, 0.1f));
    // Congela a animação exclusivamente no último frame (o túmulo)
    anim->AddAnimation("dead", Animation(11, 11, 1.0f));
    
    // Força a engine a recortar e mostrar o primeiro frame imediatamente!
    anim->SetAnimation("idle"); 
    
    associated.AddComponent(anim);
    
    // Adiciona a caixa de colisão ao jogador
    associated.AddComponent(new Collider(associated));
}

Character::~Character() {
    // Se o player global for este objeto que está a ser destruído, limpa o ponteiro
    if (player == this) {
        player = nullptr;

        // CORREÇÃO: Avisa a câmera para parar de seguir o corpo que acabou de ser apagado!
        Camera::Unfollow(); 
    }
}

void Character::Start() {
    // Cria a Gun e junta ela ao State atual
    GameObject* gunGo = new GameObject();
    
    // Passa o GameObject da Gun e um weak_ptr deste Character para a Gun o seguir
    gunGo->AddComponent(new Gun(*gunGo, Game::GetInstance().GetState().GetObjectPtr(&associated)));
    
    // Adiciona ao State e guarda o weak_ptr de volta
    gun = Game::GetInstance().GetState().AddObject(gunGo);
}

void Character::Update(float dt) {
    // Atualiza os cronômetros
    damageCooldown.Update(dt);

    // Verifica se o personagem já morreu
    if (hp <= 0) {
        // Limpa a fila de tarefas acumuladas para não corromper a heap
        std::queue<Command> emptyQueue;
        std::swap(taskQueue, emptyQueue);
        
        // Se morreu, destroi a arma instantaneamente
        std::shared_ptr<GameObject> gunGo = gun.lock();
        if (gunGo) {
            gunGo->RequestDelete();
        }

        // Toca a animação do túmulo
        Animator* anim = (Animator*)associated.GetComponent("Animator");
        if (anim) anim->SetAnimation("dead");
        
        deathTimer.Update(dt);
        if (deathTimer.Get() > 1.0f) associated.RequestDelete();
        
        return; // Não faz mais nada se estiver morto
    }

    bool isMoving = false;

    // Processa a fila de tarefas
    while (!taskQueue.empty()) {
        Command task = taskQueue.front();
        taskQueue.pop(); // Remove a tarefa da fila

        if (task.type == MOVE) {
            speed = task.pos * linearSpeed;
            
            // Move a box uma única vez
            associated.box.x += speed.x * dt;
            associated.box.y += speed.y * dt;
            
            if (speed.GetMagnitude() > 0) {
                isMoving = true;
                
                // Espelha o Sprite se for para a esquerda
                SpriteRenderer* sr = (SpriteRenderer*)associated.GetComponent("SpriteRenderer");
                if (sr) {
                    if (speed.x < 0) sr->SetFlip(SDL_FLIP_HORIZONTAL);
                    else if (speed.x > 0) sr->SetFlip(SDL_FLIP_NONE);
                }
            }
        } 
        else if (task.type == SHOOT) {
            // Tenta obter acesso seguro à arma usando o .lock() do weak_ptr
            std::shared_ptr<GameObject> gunPtr = gun.lock();
            if (gunPtr) {
                Gun* g = (Gun*)gunPtr->GetComponent("Gun");
                if (g) {
                    g->Shoot(task.pos); // Dispara na direção do rato
                }
            }
        }
    }

    // Define a animação correta
    Animator* anim = (Animator*)associated.GetComponent("Animator");
    if (anim) {
        if (isMoving) anim->SetAnimation("walking");
        else anim->SetAnimation("idle");
    }
}

void Character::Render() {
    // Vazio, os componentes visuais já tratam disto
}

bool Character::Is(std::string type) {
    return type == "Character";
}

void Character::Issue(Command task) {
    taskQueue.push(task); // Adiciona a tarefa à fila
}

void Character::NotifyCollision(GameObject& other) {
    // Verifica se o que bateu no jogador foi um zumbi
    Zombie* zombie = (Zombie*)other.GetComponent("Zombie");
    
    if (zombie != nullptr && !other.IsDead()) {
        // Só leva dano se já passou 1 segundo desde a última mordida 
        // ou se for a primeira mordida (tempo = 0)
        if (damageCooldown.Get() >= 1.0f || damageCooldown.Get() == 0.0f) {
            
            hp -= 10; // Agora o Zumbi pode tirar mais vida por batida (ex: 10 de dano)
            
            damageCooldown.Restart(); // Reinicia o cronômetro para proteger o jogador
            damageCooldown.Update(0.001f); // Pequeno empurrãozinho para não ficar em zero
            
            // Opcional: Imprime no terminal para você ver a vida a descer
            // std::cout << "Ouch! HP do Jogador: " << hp << std::endl;
            /*if (hp <= 0) {
                // Não deletamos o personagem aqui, deixamos o Update tocar a animação!
                std::cout << "O Jogador Morreu!" << std::endl;
            }*/
        }
    }
}