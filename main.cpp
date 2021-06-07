#include <cstdlib>
#include "Graphics.h"
#include "Game.h"

int main() {
    srand(time(NULL));

    Graphics::Init();

    Game game;
    game.Start();
}
