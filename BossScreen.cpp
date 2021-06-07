#include "BossScreen.h"

#include "Game.h"

BossScreen::BossScreen(Game* game, int _difficulty)
    : game(game)
    , difficulty(_difficulty) {
    skeletonClose = Sprite("assets/skeletonClose.txt");
}

void BossScreen::Loop() {
    if (turn == -1) {
        if (difficulty == 1)
            game->Log("You hear a rumbling...");
        else
            game->Log("You hear a very familiar rumbling...");
        turn++;
    }
    else {
        Graphics::Render(skeletonClose, 18, 9);
    }

    // Render the controls box
    Graphics::DrawText("[c]ontinue", 1, 48);
}

void BossScreen::Input(char input) {
    if (difficulty < 3) {
        if (input == 'c') {
            switch (turn++) {
                case 0:
                    game->Log("\"You've finally defeated all of my skeletons in their games.\"");
                    break;
                case 1:
                    game->Log("\"But your ordeal is yet to be over...\"");
                    break;
                case 2:
                    game->Log("\"We must see, can you beat me in the game of Boned?\"");
                    break;
                case 3:
                    game->ToMap();
                    break;
            }
        }
    }
    else {
        if (input == 'c') {
            switch (turn++) {
                case 0:
                    game->Log("\"You have persevered well, traveler\"");
                    break;
                case 1:
                    game->Log("\"You have truly bested me in my own game...\"");
                    break;
                case 2:
                    game->Log("\"For that, you may be free!\"");
                    break;
                case 3:
                    game->Stop();
                    break;
            }
        }
    }
}
