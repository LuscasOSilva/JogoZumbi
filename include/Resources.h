#ifndef RESOURCES_H
#define RESOURCES_H

#include <unordered_map>
#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <iostream>

class Resources {
public:
    // Retorna a textura se já existir, ou carrega uma nova
    static SDL_Texture* GetImage(std::string file);
    static void ClearImages();

    static Mix_Music* GetMusic(std::string file);
    static void ClearMusics();

    static Mix_Chunk* GetSound(std::string file);
    static void ClearSounds();

private:
    // Tabelas que associam o caminho do arquivo ao ponteiro na memória
    static std::unordered_map<std::string, SDL_Texture*> imageTable;
    static std::unordered_map<std::string, Mix_Music*> musicTable;
    static std::unordered_map<std::string, Mix_Chunk*> soundTable;
};

#endif