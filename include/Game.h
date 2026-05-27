#ifndef GAME_H
#define GAME_H

#include <string>
#include <stack>
#include <memory>
#include "SDL2/SDL.h"
#include "State.h"

class Game {
public:
    ~Game();                               // Destrutor
    static Game& GetInstance();            // T1/T2 Método de acesso Singleton
    void Run();                            // O Game Loop

    // ALTERADO: Agora retorna uma referência para o Estado no topo da pilha
    State& GetState();

    // NOVO: Método para empilhar um novo estado
    void Push(State* state);

    SDL_Renderer* GetRenderer();           // Getter do renderizador
    float GetDeltaTime();
    void CalculateDeltaTime();
    
private:
    // Construtor privado: recebe título e dimensões [cite: 
    Game(std::string title, int width, int height);
    static Game* instance;                 // Instância única estática
    
    SDL_Window* window;                    // Janela da SDL
    SDL_Renderer* renderer;                // Renderizador da SDL

    float dt;
    int frameStart;

    std::stack<std::unique_ptr<State>> stateStack;
    State* storedState; // Guarda o estado que aguarda para ser empilhado
};

#endif