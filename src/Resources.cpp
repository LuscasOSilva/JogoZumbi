#include "Resources.h"
#include "Game.h"
#include "SDL2/SDL_image.h"
#include <iostream>

// Inicialização dos membros estáticos (obrigatório em C++)
std::unordered_map<std::string, SDL_Texture*> Resources::imageTable;
std::unordered_map<std::string, Mix_Music*> Resources::musicTable;
std::unordered_map<std::string, Mix_Chunk*> Resources::soundTable;

SDL_Texture* Resources::GetImage(std::string file) {
    if (imageTable.find(file) != imageTable.end()) return imageTable[file];
    
    SDL_Texture* tex = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());
    if (tex) imageTable[file] = tex;
    return tex;
}

Mix_Music* Resources::GetMusic(std::string file) {
    auto it = musicTable.find(file);
    if (it != musicTable.end()) {
        return it->second;
    }

    Mix_Music* music = Mix_LoadMUS(file.c_str());
    if (music == nullptr) {
        std::cerr << "Erro ao carregar música: " << Mix_GetError() << std::endl;
        return nullptr;
    }

    musicTable[file] = music;
    return music;
}

// --- GERENCIAMENTO DE SONS (CHUNKS)
Mix_Chunk* Resources::GetSound(std::string file) {
    if (soundTable.find(file) != soundTable.end()) return soundTable[file];
    
    Mix_Chunk* chunk = Mix_LoadWAV(file.c_str());
    if (chunk) soundTable[file] = chunk;
    return chunk;
}

void Resources::ClearImages() {
    // Percorre o mapa de texturas
    for (auto const& [key, texture] : imageTable) {
        if (texture != nullptr) {
            SDL_DestroyTexture(texture); // Desaloca a textura da GPU
        }
    }
    imageTable.clear(); // Limpa as entradas do mapa
}

void Resources::ClearMusics() {
    // Percorre o mapa de músicas
    for (auto const& [key, music] : musicTable) {
        if (music != nullptr) {
            Mix_FreeMusic(music); // Libera a música da memória
        }
    }
    musicTable.clear(); // Limpa as entradas do mapa
}

void Resources::ClearSounds() {
    // Percorre o mapa de efeitos sonoros (chunks)
    for (auto const& [key, chunk] : soundTable) {
        if (chunk != nullptr) {
            Mix_FreeChunk(chunk); // Libera o som da memória
        }
    }
    soundTable.clear(); // Limpa as entradas do mapa
}