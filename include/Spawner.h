#ifndef SPAWNER_H
#define SPAWNER_H

#include "Component.h"
#include "Timer.h"

class Spawner : public Component {
public:
    Spawner(GameObject& associated, float cooldownTime);
    
    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) override;

private:
    Timer timer;
    float cooldownTime;
};

#endif