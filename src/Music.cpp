#include "Music.h"
#include "Resources.h"
#include <iostream>

Music::Music() {
    music = nullptr; // Inicializa vazio
}

Music::Music(std::string file) {
    music = nullptr;
    Open(file);
}

Music::~Music() {
    Stop(); // Para imediatamente antes de liberar
    /*if (music != nullptr) {
        Mix_FreeMusic(music); // Desaloca da memória
    }*/
}

void Music::Open(std::string file) {
    // Carrega o arquivo de música
    music = Resources::GetMusic(file);
    if (music == nullptr) {
        std::cerr << "Erro ao carregar musica: " << SDL_GetError() << std::endl; //
    }
}

void Music::Play(int times) {
    if (music != nullptr) {
        // Toca a música. O Mixer só toca uma por vez
        Mix_PlayMusic(music, times);
    }
}

void Music::Stop(int msToStop) {
    // Para a música com efeito de fade-out
    Mix_FadeOutMusic(msToStop);
}

bool Music::IsOpen() {
    return music != nullptr;
}