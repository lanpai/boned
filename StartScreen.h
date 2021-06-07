#pragma once

#include "Screen.h"
#include "Graphics.h"

class Game;

class StartScreen : public Screen {
    public:
        StartScreen(Game* game);

        void Loop();
        void Input(char input);

    private:
        Game* game;
        int turn = 0;

        Sprite boned;
        Sprite candles;
        Sprite bone;
};
