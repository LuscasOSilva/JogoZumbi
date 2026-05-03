#ifndef TILESET_H
#define TILESET_H

#include <string>
#include "Sprite.h"

class TileSet {
public:
    TileSet(int tileWidth, int tileHeight, std::string file);
    
    void RenderTile(unsigned index, float x, float y); // Desenha o tile específico
    
    int GetTileWidth();
    int GetTileHeight();

private:
    Sprite tileSet; // A imagem que contém todos os tiles
    int tileWidth;
    int tileHeight;
    int tileCount;  // Quantidade total de tiles na imagem
};

#endif