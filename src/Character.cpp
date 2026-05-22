#include "Character.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Game.h"
#include "Gun.h"
#include "State.h"
#include "GameObject.h"

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
    
    /* ATENÇÃO: Descomente e ajuste as linhas abaixo dependendo de 
       como você declarou a função AddAnimation no seu Animator.h!
       A assinatura costuma ser: (nome, frameStart, frameEnd, frameTime)
    */
    // anim->AddAnimation("idle", 0, 0, 1.0f);
    // anim->AddAnimation("walking", 0, 5, 0.1f); // Ajuste os frames corretos do andar
    // anim->AddAnimation("dead", 10, 11, 0.2f);  // Os 2 túmulos que você mencionou
    
    associated.AddComponent(anim);
}

Character::~Character() {
    // Se o player global for este objeto que está a ser destruído, limpa o ponteiro
    if (player == this) {
        player = nullptr;
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
    // Verifica se o personagem já morreu
    if (hp <= 0) {
        Animator* anim = (Animator*)associated.GetComponent("Animator");
        if (anim) anim->SetAnimation("dead");
        
        // deathTimer.Update(dt);
        // if (deathTimer.Get() > TEMPO_DA_ANIMACAO_DE_MORTE) associated.RequestDelete();
        
        return; // Não faz mais nada se estiver morto
    }

    bool isMoving = false;

    // Processa a fila de tarefas
    while (!taskQueue.empty()) {
        Command task = taskQueue.front();
        taskQueue.pop(); // Remove a tarefa da fila

        if (task.type == MOVE) {
            // task.pos aqui funciona como um vetor de direção normalizado (tamanho 1)
            speed = task.pos * linearSpeed;
            
            // Move a box
            associated.box.x += speed.x * dt;
            associated.box.y += speed.y * dt;
            
            if (speed.GetMagnitude() > 0) isMoving = true;
            
            // Opcional: Se for para a esquerda, espelha o sprite
            // SpriteRenderer* sr = (SpriteRenderer*)associated.GetComponent("SpriteRenderer");
            // se speed.x < 0, sr->SetFlip(SDL_FLIP_HORIZONTAL); etc...
            
        } else if (task.type == SHOOT) {
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