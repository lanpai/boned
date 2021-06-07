#pragma once

#include "Skeleton.h"
#include "Graphics.h"

class Game;

class SkeletonTTT : public Skeleton {
    public:
        SkeletonTTT(Game* game, int difficulty);

        void Loop();
        void Input(char input);

        int CheckWinCon();
        int GetWinCon();
        void SkeletonTurn();

        void DrawBoard(int x, int y);
        void ClearBoard();
        string GetCursorPos();

    private:
        Game* game;
        int difficulty;
        int turn = 0;
        int** board; // 1=Player=o, 2=Skeleton=x

        int cursorX, cursorY;
        
        Sprite skeletonClose;
        Sprite skeleton;
        Sprite O;
        Sprite X;
};
