#pragma once

#include "Skeleton.h"
#include "Graphics.h"

#define C4_HEIGHT 5

class Game;

class SkeletonC4 : public Skeleton {
    public:
        SkeletonC4(Game* game, int difficulty);

        void Loop();
        void Input(char input);

        void PlacePiece(int x, int marker);

        int CheckWinCon();
        int GetWinCon();
        void SkeletonTurn();

        void DrawBoard(int x, int y);
        void ClearBoard();

    private:
        Game* game;
        int difficulty;
        int turn = 0;
        int** board; // 1=Player, 2=Skeleton
        int width;

        int cursor;

        Sprite skeletonClose;
        Sprite skeleton;
        Sprite skeletonPiece;
        Sprite playerPiece;
};
