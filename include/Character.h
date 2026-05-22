#ifndef CHARACTER_H
#define CHARACTER_H

#include "Component.h"
#include "Vec2.h"
#include <string>
#include <queue>
#include <memory>
#include "Timer.h" 

class Character : public Component {
public:
    // Enumeração dos tipos de comando permitidos
    enum CommandType { MOVE, SHOOT };

    // Classe pública Command embutida no Character
    class Command {
    public:
        CommandType type;
        Vec2 pos;
        Command(CommandType type, float x, float y) : type(type), pos(x, y) {}
    };

    Character(GameObject& associated, std::string sprite);
    ~Character();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) override;

    void Issue(Command task);

    static Character* player; // Ponteiro global para facilitar o acesso pelos inimigos

private:
    std::weak_ptr<GameObject> gun;
    std::queue<Command> taskQueue;
    Vec2 speed;
    float linearSpeed;
    int hp;
    
    Timer deathTimer;
};

#endif