#pragma once

#include "Screen.h"
#include "Graphics.h"

class Game;

class VictoryScreen : public Screen {
    public:
        VictoryScreen(Game* game);

        void Loop();
        void Input(char input);

    private:
        Game* game;

        Sprite victory;
};
