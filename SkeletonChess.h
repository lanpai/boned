#pragma once

#include "Skeleton.h"
#include "Graphics.h"

class Game;

class SkeletonChess : public Skeleton {
    public:
        SkeletonChess(Game* game, int difficulty);

        void Loop();
        void Input(char input);

        void SelectNext();
        void SelectPrev();

        bool CheckValidity(int x1, int y1, int x2, int y2);
        int IdentifyPiece(int x, int y);

        int CheckWinCon();

        void ImportPartialFEN(string fen);
        string ExportPartialFEN();

        void DrawBoard(int x, int y);

        string GetCursorPos(int x, int y);
        string GetPieceName(char type);

    private:
        Game* game;
        int difficulty;
        int turn = 0;

        char** board;
        string solution;

        int cursorX, cursorY;
        bool inSelectMode = true;
        int selectX, selectY;

        Sprite skeletonClose;
        Sprite skeleton;
};
