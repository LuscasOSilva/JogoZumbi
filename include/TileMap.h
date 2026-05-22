#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <string>
#include <memory>
#include "Component.h"
#include "TileSet.h"

class TileMap : public Component {
public:
    TileMap(GameObject& associated, std::string file, TileSet* tileSet);
    
    void Load(std::string file); // Carrega o mapa do arquivo
    void SetTileSet(TileSet* tileSet);
    int& At(int x, int y, int z = 0); // Acessa elemento da matriz
    
    void Render() override;
    void RenderLayer(int layer, int cameraX = 0, int cameraY = 0); // Renderiza camada
    
    int GetWidth();
    int GetHeight();
    int GetDepth();
    
    void Update(float dt) override;
    
    bool Is(std::string type) override;

private:
    std::vector<int> tileMatrix; // Vetor que simula a matriz 3D
    std::unique_ptr<TileSet> tileSet; // Ponteiro para o conjunto de tiles
    int mapWidth;
    int mapHeight;
    int mapDepth;
};

#endif