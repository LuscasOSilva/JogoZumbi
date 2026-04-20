#ifndef MUSIC_H
#define MUSIC_H

#include "SDL2/SDL_mixer.h"
#include <string>

class Music {
public:
    Music();                         // Construtor padrão
    Music(std::string file);         // Construtor que abre arquivo
    ~Music();                        // Destrutor

    void Play(int times = -1);       // Toca a música (default infinito)
    void Stop(int msToStop = 1500);  // Para com efeito fade-out
    void Open(std::string file);     // Carrega do disco
    bool IsOpen();                   // Checa se está carregada

private:
    Mix_Music* music;                // Ponteiro para a música da SDL_mixer
};

#endif