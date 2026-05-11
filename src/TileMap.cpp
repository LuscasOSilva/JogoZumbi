#include "TileMap.h"
#include "GameObject.h"
#include "Camera.h"
#include <fstream>
#include <iostream>

TileMap::TileMap(GameObject& associated, std::string file, TileSet* tileSet) 
    : Component(associated), tileSet(tileSet) {
    Load(file);
}

void TileMap::Load(std::string file) {
    tileMatrix.clear(); //Limpa o mapa antigo antes de ler o novo

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
        tileMatrix.push_back(tileIndex); 
    }
}

int& TileMap::At(int x, int y, int z) {
    // Fórmula para mapear 3D em 1D: x + y*W + z*W*H
    return tileMatrix[x + (y * mapWidth) + (z * mapWidth * mapHeight)];
}

void TileMap::RenderLayer(int layer, int cameraX, int cameraY) {
    int tw = tileSet->GetTileWidth(); //64
    int th = tileSet->GetTileHeight();  //64

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int index = At(x, y, layer);
            if (index >= 0) {
                // Garante que não renderiza tiles vazios (-1)
                // O tile deve respeitar a posição do mapObj (associated.box)
                // Renderiza considerando a posição do GameObject (box)
                tileSet->RenderTile(index, 
                                    (x * tw) + associated.box.x - Camera::pos.x, 
                                    (y * th) + associated.box.y - Camera::pos.y);
            }
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