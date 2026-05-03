#include "TileSet.h"

TileSet::TileSet(int tileWidth, int tileHeight, std::string file) 
    : tileSet(file), tileWidth(tileWidth), tileHeight(tileHeight) {
    
    // Calcula quantos tiles cabem na imagem
    int columns = tileSet.GetWidth() / tileWidth;
    int rows = tileSet.GetHeight() / tileHeight;
    tileCount = columns * rows;
    
    // Define a contagem de frames na sprite para facilitar o recorte
    tileSet.SetFrameCount(columns, rows);
}

void TileSet::RenderTile(unsigned index, float x, float y) {
    // Checa se o índice é válido (entre 0 e tileCount - 1)
    if (index < (unsigned)tileCount) {
        tileSet.SetFrame(index); // Seleciona o tile
        tileSet.Render(x, y);    // Desenha na posição desejada
    }
}

int TileSet::GetTileWidth() { return tileWidth; }
int TileSet::GetTileHeight() { return tileHeight; }