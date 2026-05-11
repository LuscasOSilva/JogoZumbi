#ifndef GAME_H
#define GAME_H

#include "SDL2/SDL.h" // Importante para o g++ no WSL 
#include <string>

// Forward declaration da classe State que será criada depois
class State;

class Game {
public:
    ~Game();                               // Destrutor
    void Run();                            // O Game Loop
    SDL_Renderer* GetRenderer();           // Getter do renderizador
    State& GetState();                     // Getter do estado atual
    static Game& GetInstance();            // Método de acesso Singleton
    void CalculateDeltaTime();
    float GetDeltaTime();

private:
    // Construtor privado: recebe título e dimensões [cite: 
    Game(std::string title, int width, int height);

    static Game* instance;                 // Instância única estática
    SDL_Window* window;                    // Janela da SDL
    SDL_Renderer* renderer;                // Renderizador da SDL
    State* state;                          // Estado atual do jogo

    float dt;
    int frameStart;
};

#endif