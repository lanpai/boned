#pragma once

#include <string>
#include "Map.h"
#include "BossScreen.h"
#include "StartScreen.h"
#include "VictoryScreen.h"
#include "DefeatScreen.h"

using namespace std;

class Skeleton;

class Game {
    public:
        Game(int difficulty=0);
        ~Game();
        void Start();
        void Stop();
        void Loop();
        void Log(const string& text);

        void To(Screen* screen);
        void ToMap();
        void ToVictory(Skeleton* skeleton);
        void ToDefeat(Skeleton* skeleton);

        void Restart(int deltaDiff = 0);

    private:
        int difficulty = 0;
        bool isPlaying = true;
        string log[5];

        Screen* activeScreen = nullptr;
        Map* map;
        BossScreen* boss;
        StartScreen* start;
        VictoryScreen* victory;
        DefeatScreen* defeat;
};
