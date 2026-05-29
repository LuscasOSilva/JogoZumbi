#include "Sound.h"
#include "Resources.h"
#include <iostream>

Sound::Sound(GameObject& associated) : Component(associated) {
    chunk = nullptr;
}

Sound::Sound(GameObject& associated, std::string file) : Sound(associated) {
    Open(file);
}

void Sound::Open(std::string file) {
    chunk = Resources::GetSound(file);
    if (chunk == nullptr) {
        std::cerr << "Erro ao carregar som: " << SDL_GetError() << std::endl;
    }
}

void Sound::Play(int times) {
    if (chunk != nullptr) {
        // Mix_PlayChannel(-1, ...) retorna o canal escolhido
        // loops = times - 1 (se times=1, loops=0 toca uma vez)
        channel = Mix_PlayChannel(-1, chunk, times - 1);
    }
}

void Sound::Stop() {
    if (chunk != nullptr) {
        Mix_HaltChannel(channel); // Para o canal específico
    }
}

Sound::~Sound() {
    if (chunk != nullptr) {
        Stop();
        /*Mix_HaltChannel(channel); // Garante que pare antes de liberar
        Mix_FreeChunk(chunk);     // Desaloca a memória*/
    }
}

// Métodos obrigatórios vazios, pois som não tem lógica de frame ou desenho
void Sound::Update(float dt) {}
void Sound::Render() {}
bool Sound::IsOpen() { return chunk != nullptr; }

bool Sound::Is(std::string type) { return type == "Sound"; }