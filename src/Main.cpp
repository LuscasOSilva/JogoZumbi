#include "Game.h"

int main (int argc, char** argv) {
    // Silencia avisos de parâmetros não utilizados (opcional)
    (void)argc;
    (void)argv;

    // Obtém a instância única do jogo (Singleton)
    Game& game = Game::GetInstance();

    // Inicia o Game Loop principal
    game.Run();

    return 0;
}