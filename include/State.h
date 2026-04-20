#ifndef STATE_H
#define STATE_H

#include "Sprite.h"
#include "Music.h"

class State {
public:
    State();               // Construtor
    ~State();              // Destrutor
    bool QuitRequested();  // Retorna se o jogo deve fechar
    void LoadAssets();     // Carrega imagens e sons
    void Update(float dt); // Atualiza a lógica
    void Render();         // Desenha no ecrã

private:
    Sprite bg;             // Imagem de fundo
    Music music;           // Música de fundo
    bool quitRequested;    // Flag de encerramento
};

#endif