#include "Game.h"
#include "State.h"
#include "Resources.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "InputManager.h"
#include <iostream>

// Inicialização da instância estática como nula
Game* Game::instance = nullptr;

Game& Game::GetInstance() {
    if (instance != nullptr) {
        return *instance; // Retorna a instância se já existir
    }
    // Se não existir, cria a única instância
    // O PDF pede 1200x900 e nome/matrícula
    instance = new Game("Lucas - 200022857", 1200, 900);
    return *instance;
}

Game::Game(std::string title, int width, int height) {
    // Garantia do Singleton
    if (instance != nullptr) {
        std::cerr << "Erro: Game ja possui uma instancia rodando!" << std::endl;
        exit(1); 
    }
    instance = this;

    // 1. Inicializa SDL
    // Retorna != 0 em caso de falha
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        std::cerr << "Erro SDL_Init: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // 2. Inicializa SDL_image
    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
        std::cerr << "Erro IMG_Init: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // 3. Inicializa SDL_mixer
    int mixFlags = MIX_INIT_FLAC | MIX_INIT_OGG | MIX_INIT_MP3;
    if ((Mix_Init(mixFlags) & mixFlags) != mixFlags) {
        std::cerr << "Erro Mix_Init: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Configuração obrigatória do Mix_OpenAudio 
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0) {
        std::cerr << "Erro Mix_OpenAudio: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Aloca 32 canais de audio conforme solicitado
    Mix_AllocateChannels(32);

    // 4. Cria a Janela
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == nullptr) {
        std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // 5. Cria o Renderizador acelerado
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Erro ao criar renderizador: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // 6. Instancia o State
    state = new State();
    // Trabalho 4
    frameStart = SDL_GetTicks();
    dt = 0;
} // Fim do construtor

Game::~Game() { // Destrutor
    // Ordem inversa da inicializacao
    // 1. Limpa o estado (quando ele existir)
    if (state != nullptr) {
        delete state; //  Libera o estado primeiro
    }

    // 2. Fecha audio
    Mix_CloseAudio();
    Mix_Quit();
    // 3. Fecha imagens
    IMG_Quit();
    // 4. Destroi renderizador e janela
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // 5. Sai da SDL
    SDL_Quit();
}

void Game::CalculateDeltaTime() {
    int currentFrameTime = SDL_GetTicks();
    // dt é o tempo atual menos o tempo do frame passado, convertido para segundos
    dt = (currentFrameTime - frameStart) / 1000.0f; // Converte ms para segundos
    frameStart = currentFrameTime;
}

float Game::GetDeltaTime() {
    return dt;
}

SDL_Renderer* Game::GetRenderer() {
    return renderer;
}

State& Game::GetState() {
    return *state;
}


void Game::Run() {
    // Ativa o Start do estado inicial antes de começar o loop
    state->Start(); 

    while (!state->QuitRequested() && !InputManager::GetInstance().QuitRequested()) {
        CalculateDeltaTime();
        InputManager::GetInstance().Update();

        state->Update(dt);
        state->Render();

        SDL_RenderPresent(renderer);
    }

    // Limpeza de recursos (Trabalho 3)
    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();
}