#include "Resources.h"
#include "Game.h"
#include "SDL2/SDL_image.h"
#include <iostream>

// Inicialização dos membros estáticos (obrigatório em C++)
std::unordered_map<std::string, SDL_Texture*> Resources::imageTable;
std::unordered_map<std::string, Mix_Music*> Resources::musicTable;
std::unordered_map<std::string, Mix_Chunk*> Resources::soundTable;

SDL_Texture* Resources::GetImage(std::string file) {
    // Tenta encontrar o arquivo na tabela
    auto it = imageTable.find(file);
    if (it != imageTable.end()) {
        return it->second; // Já estava carregado!
    }

    // Não estava carregado, vamos carregar agora
    SDL_Texture* texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());
    if (texture != nullptr) {
        imageTable[file] = texture; // Guarda para a próxima vez
    }
    return texture;
}

void Resources::ClearImages() {
    // Percorre a tabela destruindo as texturas reais
    for (auto& pair : imageTable) {
        SDL_DestroyTexture(pair.second);
    }
    imageTable.clear(); // Esvazia a tabela
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

void Resources::ClearMusics() {
    for (auto& pair : musicTable) {
        Mix_FreeMusic(pair.second); // Libera a memória da música
    }
    musicTable.clear();
}

// --- GERENCIAMENTO DE SONS (CHUNKS)
Mix_Chunk* Resources::GetSound(std::string file) {
    auto it = soundTable.find(file);
    if (it != soundTable.end()) {
        return it->second;
    }

    Mix_Chunk* chunk = Mix_LoadWAV(file.c_str());
    if (chunk == nullptr) {
        std::cerr << "Erro ao carregar som: " << Mix_GetError() << std::endl;
        return nullptr;
    }

    soundTable[file] = chunk;
    return chunk;
}

void Resources::ClearSounds() {
    for (auto& pair : soundTable) {
        Mix_FreeChunk(pair.second); // Libera o som da memória
    }
    soundTable.clear();
}