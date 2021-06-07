#include "DefeatScreen.h"

#include "Game.h"

DefeatScreen::DefeatScreen(Game* game)
    : game(game) {
    defeat = Sprite("assets/defeat.txt");
}

void DefeatScreen::Loop() {
    Graphics::Render(defeat, 14, 1);

    // Render the controls box
    Graphics::DrawText("[c]ontinue", 1, 48);
}

void DefeatScreen::Input(char input) {
    if (input == 'c')
        game->Restart(-1);
}
