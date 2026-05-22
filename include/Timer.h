#ifndef TIMER_H
#define TIMER_H

class Timer {
public:
    Timer();
    
    // Soma o dt ao tempo acumulado
    void Update(float dt);
    
    // Volta o tempo para zero
    void Restart();
    
    // Retorna o tempo atual
    float Get();

private:
    float time;
};

#endif