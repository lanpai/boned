#include "SkeletonTTT.h"

#include <cstdlib>
#include "Game.h"

SkeletonTTT::SkeletonTTT(Game* game, int difficulty)
    : game(game)
    , difficulty(difficulty) {
    cursorX = 0;
    cursorY = 0;

    board = new int*[difficulty + 3];
    for (int y = 0; y < difficulty + 3; y++) {
        board[y] = new int[difficulty + 3];
        for (int x = 0; x < difficulty + 3; x++)
            board[y][x] = 0;
    }

    skeletonClose = Sprite("assets/skeletonClose.txt");
    skeleton = Sprite("assets/skeleton.txt");
    O = Sprite("assets/o.txt");
    X = Sprite("assets/x.txt");
}

void SkeletonTTT::Loop() {
    if (turn == 0) {
        // Render the controls box
        Graphics::DrawText("[c]ontinue", 1, 48);

        Graphics::Render(skeletonClose, 18, 9);

        game->Log("\"A challenger approaches to test my tic-tac-toe!\"");
    }
    else {
        DrawBoard(50 - (6*(difficulty + 3))/2, 21 - (5*(difficulty + 3))/2);
        Graphics::Render(skeleton, 4, 29);

        if (turn > 0) {
            Graphics::DrawText("Turn: " + to_string(turn), 1, 1);

            // Render the controls box
            string controls = "move [u]p   move [l]eft   move [d]own   move [r]ight   [p]lace an O at ";
            controls += GetCursorPos();
            Graphics::DrawText(controls, 1, 48);
        }
        else
            Graphics::DrawText("[c]ontinue", 1, 48);
    }
}

void SkeletonTTT::Input(char input) {
    if (input == '9')
        turn = -2;
    else if (input == '0')
        turn = -3;

    if (turn == 0) {
        if (input == 'c')
            turn++;
    }
    else if (turn == -2) {
        if (input == 'c')
            game->ToVictory(this);
    }
    else if (turn == -3) {
        if (input == 'c')
            game->ToDefeat(this);
    }
    else {
        switch (input) {
            case 'u':
                cursorY = max(cursorY - 1, 0);
                game->Log("You moved your cursor to " + GetCursorPos());
                break;
            case 'l':
                cursorX = max(cursorX - 1, 0);
                game->Log("You moved your cursor to " + GetCursorPos());
                break;
            case 'd':
                cursorY = min(cursorY + 1, difficulty + 2);
                game->Log("You moved your cursor to " + GetCursorPos());
                break;
            case 'r':
                cursorX = min(cursorX + 1, difficulty + 2);
                game->Log("You moved your cursor to " + GetCursorPos());
                break;
            case 'p':
                if (board[cursorY][cursorX] == 0) {
                    board[cursorY][cursorX] = 1;
                    game->Log("You placed an O at " + GetCursorPos());
                    if (!CheckWinCon())
                        SkeletonTurn();
                }
                else {
                    game->Log("There's already a marker at " + GetCursorPos());
                }
                break;
        }
    }
}

int SkeletonTTT::CheckWinCon() {
    switch (GetWinCon()) {
        default:
        case 0:
            return 0;
        case 1:
            game->Log("\"A true challenge you are!\"");
            turn = -2;
            return 1;
        case 2:
            game->Log("\"Disapointing... you posed me no challenge.\"");
            turn = -3;
            return 2;
        case 3:
            game->Log("\"It seems we tied! You are a true test of skill!\"");
            ClearBoard();
            return 3;
    }
}

int SkeletonTTT::GetWinCon() {
    // Horizontal checks
    for (int y = 0; y < difficulty + 3; y++) {
        bool meetsWinCon = true;
        int marker = board[y][0];
        if (marker) {
            for (int x = 0; x < difficulty + 3; x++)
                if (marker != board[y][x]) {
                    meetsWinCon = false;
                    break;
                }
            if (meetsWinCon)
                return marker;
        }
    }

    // Vertical checks
    for (int x = 0; x < difficulty + 3; x++) {
        bool meetsWinCon = true;
        int marker = board[0][x];
        if (marker) {
            for (int y = 0; y < difficulty + 3; y++)
                if (marker != board[y][x]) {
                    meetsWinCon = false;
                    break;
                }
            if (meetsWinCon)
                return marker;
        }
    }

    // Diagonal checks
    {
        bool meetsWinCon = true;
        int marker = board[0][0];
        if (marker) {
            for (int i = 0; i < difficulty + 3; i++)
                if (marker != board[i][i]) {
                    meetsWinCon = false;
                    break;
                }
            if (meetsWinCon)
                return marker;
        }
    }
    {
        bool meetsWinCon = true;
        int marker = board[difficulty + 2][0];
        if (marker) {
            for (int i = 0; i < difficulty + 3; i++)
                if (marker != board[difficulty + 2 - i][i]) {
                    meetsWinCon = false;
                    break;
                }
            if (meetsWinCon)
                return marker;
        }
    }

    // Tie check
    bool hasEmpty = false;
    for (int y = 0; y < difficulty + 3; y++)
        for (int x = 0; x < difficulty + 3; x++)
            if (board[y][x] == 0) {
                hasEmpty = true;
                break;
            }
    if (!hasEmpty)
        return 3;

    return 0;
}

void SkeletonTTT::SkeletonTurn() {
    int moveCount = 0;
    for (int y = 0; y < difficulty + 3; y++)
        for (int x = 0; x < difficulty + 3; x++)
            if (board[y][x] == 0)
                moveCount++;

    int move = rand() % moveCount;
    for (int y = 0; y < difficulty + 3; y++)
        for (int x = 0; x < difficulty + 3; x++)
            if (board[y][x] == 0)
                if (move-- == 0) {
                    board[y][x] = 2;
                    if (!CheckWinCon())
                        turn++;
                    return;
                }
}

void SkeletonTTT::DrawBoard(int _x, int _y) {
    for (int y = 0; y < difficulty + 3; y++) {
        for (int x = 0; x < difficulty + 3; x++) {
            // Draw frame
            if (y != 0)
                Graphics::DrawLine(_x + 6*x, _y + 5*y, 7, false);
            if (x != 0)
                Graphics::DrawLine(_x + 6*x, _y + 5*y, 5, true);

            // Draw markers
            switch (board[y][x]) {
                case 0:
                    break;
                case 1:
                    Graphics::Render(O, _x + 6*x + 1, _y + 5*y);
                    break;
                case 2:
                    Graphics::Render(X, _x + 6*x + 1, _y + 5*y);
                    break;
            }
        }
    }

    // Draw labels
    for (int y = 0; y < difficulty + 3; y++)
        Graphics::DrawText(string(1,'1'+y), _x - 1, _y + 5*y + 2);
    for (int x = 0; x < difficulty + 3; x++)
        Graphics::DrawText(string(1,'A'+x), _x + 6*x + 3, _y - 1);
}

void SkeletonTTT::ClearBoard() {
    for (int y = 0; y < difficulty + 3; y++)
        for (int x = 0; x < difficulty + 3; x++)
            board[y][x] = 0;
}

string SkeletonTTT::GetCursorPos() {
    return string(1,'A'+cursorX) + string(1,'1'+cursorY);
}
