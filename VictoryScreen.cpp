#include "VictoryScreen.h"

#include "Game.h"

VictoryScreen::VictoryScreen(Game* game)
    : game(game) {
    victory = Sprite("assets/victory.txt");
}

void VictoryScreen::Loop() {
    Graphics::Render(victory, 5, 1);

    // Render the controls box
    Graphics::DrawText("[c]ontinue", 1, 48);
}

void VictoryScreen::Input(char input) {
    if (input == 'c')
        game->ToMap();
}
