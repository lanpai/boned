#include "StartScreen.h"

#include "Game.h"

StartScreen::StartScreen(Game* game)
    : game(game) {
    boned = Sprite("assets/boned.txt");
    candles = Sprite("assets/candles.txt");
    bone = Sprite("assets/bone.txt");
}

void StartScreen::Loop() {
    if (turn == 0) {
        Graphics::Render(boned, 20, 3);
        Graphics::Render(candles, 21, 20);

        // Render the controls box
        Graphics::DrawText("[s]tart game", 1, 48);
    }
    else {
        switch (turn) {
            case 1:
                Graphics::Render(candles, 21, 20);
                break;
            case 2:
                Graphics::Render(bone, 37, 38);
                break;
        }

        // Render the controls box
        Graphics::DrawText("[c]ontinue", 1, 48);
    }
}

void StartScreen::Input(char input) {
    if (turn == 0) {
        if (input == 's') {
            game->Log("There are few candles to pave your way...");
            turn++;
        }
    }
    else {
        if (input == 'c') {
            switch (turn++) {
                case 1:
                    game->Log("You hear the faint sound of bones rattling in the distance...");
                    break;
                case 2:
                    game->Log("Paying it little mind, you move into the darkness...");
                    break;
                case 3:
                    game->ToMap();
                    break;
            }
        }
    }
}
