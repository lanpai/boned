#include "Graphics.h"

#include <iostream>
#include <fstream>
#include <cmath>

Sprite::Sprite(int _width, int _height)
    : width(_width)
    , height(_height) {
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            pixels[y][x] = ' ';
}

Sprite::Sprite(string path) {
    ifstream input(path);

    for (int y = 0; y < 255; y++)
        for (int x = 0; x < 255; x++)
            pixels[y][x] = ' ';

    width = 0;
    height = 0;

    string line;
    while (getline(input, line)) {
        height++;

        int lineWidth = 0;
        for (char pixel : line) {
            pixels[height][lineWidth++] = pixel;
        }
        width = max(lineWidth, width);
    }
}

char Graphics::display[HEIGHT][WIDTH];

void Graphics::Init() {
    ClearFrame();
}

void Graphics::ClearFrame() {
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            display[y][x] = ' ';
}

void Graphics::ShowFrame() {
    // TODO: Clear terminal

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++)
            cout << display[y][x];
        cout << endl;
    }
}

void Graphics::Render(const Sprite& sprite, int _x, int _y) {
    for (int y = _y; y < min(_y + sprite.height, HEIGHT); y++)
        for (int x = _x; x < min(_x + sprite.width, WIDTH); x++) {
            if (sprite.pixels[y - _y][x - _x] != ' ')
                display[y][x] = sprite.pixels[y - _y][x - _x];
        }
}

void Graphics::DrawLine(int _x, int _y, int delta, bool isVertical) {
    if (isVertical) {
        for (int y = _y; y < _y + delta; y++)
            display[y][_x] = '|';
        display[_y + delta][_x] = '+';
    }
    else {
        for (int x = _x; x < _x + delta; x++)
            display[_y][x] = '-';
        display[_y][_x + delta - 1] = '+';
    }
    display[_y][_x] = '+';
}

void Graphics::DrawBox(int x1, int y1, int x2, int y2) {
    for (int x = x1; x <= x2; x++) {
        display[y1][x] = '-';
        display[y2][x] = '-';
    }
    for (int y = y1; y <= y2; y++) {
        display[y][x1] = '|';
        display[y][x2] = '|';
    }
    display[y1][x1] = '+';
    display[y1][x2] = '+';
    display[y2][x1] = '+';
    display[y2][x2] = '+';
}

void Graphics::DrawText(const string& text, int x, int y, bool isVertical) {
    int i = 0;
    for (char pixel : text) {
        if (isVertical) {
            display[y + i++][x] = pixel;
            if (y + i >= HEIGHT) break;
        }
        else {
            display[y][x + i++] = pixel;
            if (x + i >= WIDTH) break;
        }
    }
}

void Graphics::DrawPixel(char pixel, int x, int y) {
    display[y][x] = pixel;
}
