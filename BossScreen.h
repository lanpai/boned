#pragma once

#include "Screen.h"
#include "Graphics.h"

class Game;

class BossScreen : public Screen {
    public:
        BossScreen(Game* game, int difficulty);

        void Loop();
        void Input(char input);

    private:
        Game* game;
        int difficulty;
        int turn = -1;

        Sprite skeletonClose;
};
