#pragma once

#include <string>
#include <vector>

using namespace std;

#define WIDTH 100
#define HEIGHT 50

struct Sprite {
    Sprite(int width=255, int height=255);
    Sprite(string path);

    int width, height;
    char pixels[255][255];
};

class Graphics {
    public:
        static void Init();
        static void ClearFrame();
        static void ShowFrame();

        static void Render(const Sprite& sprite, int x, int y);
        static void DrawLine(int x, int y, int delta, bool isVertical = false);
        static void DrawBox(int x1, int y1, int x2, int y2);
        static void DrawText(const string& text, int x, int y, bool isVertical = false);
        static void DrawPixel(char pixel, int x, int y);

    private:
        static char display[HEIGHT][WIDTH];
};
