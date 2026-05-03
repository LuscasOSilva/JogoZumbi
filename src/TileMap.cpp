#include "TileMap.h"
#include "GameObject.h"
#include <fstream>
#include <iostream>

TileMap::TileMap(GameObject& associated, std::string file, TileSet* tileSet) 
    : Component(associated), tileSet(tileSet) {
    Load(file);
}

void TileMap::Load(std::string file) {
    std::ifstream f(file);
    if (!f.is_open()) {
        std::cerr << "Erro ao abrir mapa: " << file << std::endl;
        return;
    }

    // Lê dimensões: largura, altura e profundidade (camadas)
    char comma;
    f >> mapWidth >> comma >> mapHeight >> comma >> mapDepth >> comma;

    int tileIndex;
    while (f >> tileIndex >> comma) {
        // No formato TileD, o índice 1 pode representar vazio conforme o PDF
        tileMatrix.push_back(tileIndex - 1); 
    }
}

int& TileMap::At(int x, int y, int z) {
    // Fórmula para mapear 3D em 1D: x + y*W + z*W*H
    return tileMatrix[x + (y * mapWidth) + (z * mapWidth * mapHeight)];
}

void TileMap::RenderLayer(int layer, int cameraX, int cameraY) {
    int tw = tileSet->GetTileWidth();
    int th = tileSet->GetTileHeight();

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int tileIndex = At(x, y, layer);
            // Renderiza considerando a posição do GameObject (box)
            tileSet->RenderTile(tileIndex, 
                                x * tw + associated.box.x - cameraX, 
                                y * th + associated.box.y - cameraY);
        }
    }
}

void TileMap::Render() {
    // Renderiza todas as camadas
    for (int z = 0; z < mapDepth; z++) {
        RenderLayer(z);
    }
}

void TileMap::Update(float dt) {
    // Por enquanto, o mapa não precisa de lógica de atualização
}

// Getters obrigatórios
int TileMap::GetWidth() { return mapWidth; }
int TileMap::GetHeight() { return mapHeight; }
int TileMap::GetDepth() { return mapDepth; }

void TileMap::SetTileSet(TileSet* tileSet) { this->tileSet.reset(tileSet); }