#include "Game.h"
#include "StageState.h"
#include "Resources.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h" // LIDAR COM TEXTO
#include "InputManager.h"
#include "TitleState.h"
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

    // Inicializa a biblioteca de fontes (TTF) T7
    if (TTF_Init() != 0) {
        SDL_Log("Erro ao inicializar SDL_ttf: %s", TTF_GetError());
        exit(EXIT_FAILURE);
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
    
    // Trabalho 4
    frameStart = SDL_GetTicks();
    dt = 0;
    
    // T7
    storedState = nullptr;
    // O estado inicial absoluto do jogo agora é o Menu de Título!
    stateStack.emplace(new TitleState());

    /* //para começarmos já no jogo, apagar a parte de cima
    storedState = nullptr;
    stateStack.emplace(new StageState());*/
} // Fim do construtor

Game::~Game() { // Destrutor
    // Ordem inversa da inicializacao
    if (storedState != nullptr) {
        delete storedState;
    }
    // Esvazia a pilha de estados
    while (!stateStack.empty()) {
        stateStack.pop();
    }

    // 2. Fecha audio
    Mix_CloseAudio();
    Mix_Quit();

    // Fecha texto
    TTF_Quit();

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
    // Retorna uma referência segura para o estado que está no topo
    return *stateStack.top().get();
}

void Game::Run() {
    // 1. Dá o Start no estado inicial que está no topo
    if (!stateStack.empty()) {
        stateStack.top()->Start();
    }

    // Loop Principal da Engine
    while (!stateStack.empty() && !stateStack.top()->QuitRequested()) {
        
        // --- GERENCIAMENTO DA PILHA DE ESTADOS ---
        // A) Se o estado atual pediu para sair (Pop)
        if (stateStack.top()->PopRequested()) {
            stateStack.pop(); // Remove do topo (chama o destrutor dele automaticamente)
            
            if (!stateStack.empty()) {
                stateStack.top()->Resume(); // Acorda o estado de baixo
            }
        }
        
        // B) Se existe um novo estado aguardando para ser inserido (Push)
        if (storedState != nullptr) {
            if (!stateStack.empty()) {
                stateStack.top()->Pause(); // Adormece o estado atual
            }
            stateStack.emplace(storedState); // Coloca o novo no topo
            stateStack.top()->Start();       // Inicializa-o
            storedState = nullptr;           // Limpa o salvamento temporário
        }

        // C) Se após o Pop a pilha esvaziar, encerramos o jogo imediatamente
        if (stateStack.empty()) {
            break;
        }

        // --- SISTEMA DE TIMING & INPUT ---
        CalculateDeltaTime();
        InputManager::GetInstance().Update();

        // --- UPDATE & RENDER DO ESTADO DO TOPO ---
        stateStack.top()->Update(dt);
        
        // Limpa a tela antes de desenhar o frame atual
        SDL_RenderClear(renderer);
        
        stateStack.top()->Render();
        
        // Apresenta o frame renderizado
        SDL_RenderPresent(renderer);

        // Controla a taxa de quadros (60 FPS)
        SDL_Delay(33); 
    }
}

void Game::Push(State* state) {
    // Não empilhamos imediatamente para não quebrar o loop atual.
    // Guardamos na retaguarda para o próximo frame.
    storedState = state;
}