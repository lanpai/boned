#pragma once

#include "Screen.h"

class Game;

struct Int2 {
    int x, y;
};

class Skeleton : public Screen {
    public:
        virtual ~Skeleton() { /* null */ };

        int GetX() { return x; }
        int GetY() { return y; }
        void SetPos(int _x, int _y) {
            x = _x;
            y = _y;
        }
        void Move(int deltaX, int deltaY) {
            x += deltaX;
            y += deltaY;
        }

    private:
        int x, y;
};
