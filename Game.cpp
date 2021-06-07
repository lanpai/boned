#include "Game.h"

#include <iostream>
#include <cstdlib>
#include "Graphics.h"
#include "SkeletonTTT.h"

Game::Game(int difficulty)
    : difficulty(difficulty) { /* null */ }

Game::~Game() {
    delete map;
    delete boss;
    delete start;
    delete victory;
    delete defeat;
}


void Game::Start() {
    isPlaying = false;
    if (difficulty < 3) {
        map = new Map(this, difficulty);
        start = new StartScreen(this);
        victory = new VictoryScreen(this);
        defeat = new DefeatScreen(this);
    }
    else {
        map = nullptr;
        start = nullptr;
        victory = nullptr;
        defeat = nullptr;
    }
    boss = new BossScreen(this, difficulty);
    if (difficulty == 0) {
        To(start);
        Log("You find yourself in an unknown place...");
    }
    else {
        To(boss);
    }

    if (!isPlaying)
        Loop();
}

void Game::Stop() {
    isPlaying = false;
}

void Game::Loop() {
    isPlaying = true;
    while (isPlaying) {
        // Platform specific clear
#if defined(__linux__)
        system("clear");
#elif defined(__APPLE__)
        system("clear");
#elif defined(_WIN32)
        system("cls");
#endif

        // Basic rendering
        Graphics::ClearFrame();
        Graphics::DrawBox(0,0,99,49); // Outer box

        // Logic branching
        if (activeScreen == nullptr) {

        }
        else {
            activeScreen->Loop();
        }

        // Render the log box
        Graphics::DrawBox(0,41,99,47); // Log box
        for (int i = 0; i < 5; i++) {
            Graphics::DrawText(log[i], 1, 42+i);
        }
    
        // Render the controls box
        Graphics::DrawBox(0,47,99,49);
        //Graphics::DrawText("[h]elp", 1, 48);

        Graphics::ShowFrame();

        // User input
        char input;
        cin >> input;

        switch (input) {
            case '7':
                Restart(1);
                break;
            case '8':
                Restart(-1);
                break;
            default:
                if (activeScreen == nullptr) {
                    Log("Unknown command!");
                }
                else {
                    activeScreen->Input(input);
                }
                break;
        }
    }
}

void Game::Log(const string& text) {
    for (int i = 4; i > 0; i--)
        log[i] = log[i - 1];
    log[0] = text;
}

void Game::To(Screen* screen) {
    activeScreen = screen;
}

void Game::ToMap() {
    To(map);
}

void Game::ToVictory(Skeleton* skeleton) {
    map->DelSkeleton(skeleton);
    To(victory);
}

void Game::ToDefeat(Skeleton* skeleton) {
    map->DelSkeleton(skeleton);
    To(defeat);
}

void Game::Restart(int deltaDiff) {
    delete map;
    delete start;
    delete victory;
    delete defeat;

    if (deltaDiff == -1)
        difficulty = 0;
    else
        difficulty += deltaDiff;
    Start();
}
