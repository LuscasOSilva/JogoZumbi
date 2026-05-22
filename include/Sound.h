#ifndef SOUND_H
#define SOUND_H

#include "SDL2/SDL_mixer.h"
#include <string>
#include "Component.h"

class Sound : public Component {
public:
    Sound(GameObject& associated);
    Sound(GameObject& associated, std::string file);
    ~Sound();

    void Play(int times = 1); // Toca o som (times=1 toca uma vez)
    void Stop();
    void Open(std::string file);
    bool IsOpen();

    void Update(float dt) override; // Obrigatório implementar
    void Render() override;         // Obrigatório implementar

    bool Is(std::string type) override;

private:
    Mix_Chunk* chunk; //
    int channel;      // Canal onde o som está tocando
};

#endif