#pragma once

#include "Screen.h"
#include "Graphics.h"

class Game;

class DefeatScreen : public Screen {
    public:
        DefeatScreen(Game* game);

        void Loop();
        void Input(char input);

    private:
        Game* game;

        Sprite defeat;
};
