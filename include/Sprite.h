#ifndef SPRITE_H
#define SPRITE_H

#include "SDL2/SDL.h"
#include <string>
#include "Vec2.h"

class Sprite {
public:
    Sprite(GameObject& associated); // Construtor padrão
    Sprite(GameObject& associated, std::string file); // Construtor que já abre um arquivo
    ~Sprite();                    // Destrutor

    void Open(std::string file);  // Carrega a imagem
    void SetClip(int x, int y, int w, int h); // Define a área de corte
    void Render(int x, int y);    // Desenha na tela
    int GetWidth();               // Retorna a largura
    int GetHeight();              // Retorna a altura
    bool IsOpen();                // Checa se a textura está alocada
    void SetFrame(int frame);
    void SetFrameCount(int frameCountW, int frameCountH);
    void SetScale(float scaleX, float scaleY);
    Vec2 GetScale();

private:
    SDL_Texture* texture;         // A imagem em si
    int width;                    // Largura da imagem
    int height;                   // Altura da imagem
    SDL_Rect clipRect;            // Retângulo de clipagem (srcRect)
    int frameCountW;
    int frameCountH;
    int currentFrame;
    Vec2 scale; // Novo: Controlo de escala (x, y)
};

#endif