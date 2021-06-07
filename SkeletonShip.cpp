#include "SkeletonShip.h"

#include "Game.h"

SkeletonShip::SkeletonShip(Game* game, int _difficulty)
    : game(game)
    , difficulty(_difficulty) {
    width = 20;
    height = 8;

    cursorX = 0;
    cursorY = 0;

    boardPlayer = new char*[height];
    boardSkeleton = new char*[height];
    for (int y = 0; y < height; y++) {
        boardPlayer[y] = new char[width];
        boardSkeleton[y] = new char[width];
        for (int x = 0; x < width; x++) {
            boardPlayer[y][x] = '.';
            boardSkeleton[y][x] = '.';
        }
    }

    skeletonClose = Sprite("assets/skeletonClose.txt");
    skeleton = Sprite("assets/skeleton.txt");
}

void SkeletonShip::Loop() {
    if (turn == -1) {
        // Render the controls box
        Graphics::DrawText("[c]ontinue", 1, 48);

        Graphics::Render(skeletonClose, 18, 9);

        game->Log("\"I was a fleet admiral in my days of flesh so battleship it is!\"");
    }
    else if (turn == 0) {
        Graphics::DrawText("Setup", 1, 1);

        // Render the controls box
        string controls = "move [u]p   move [l]eft   move [d]own   move [r]ight   r[o]tate   [p]lace a ";
        controls += GetShipName(currentShip);
        controls += " at " + GetCursorPos();
        Graphics::DrawText(controls, 1, 48);

        DrawBoard(50 - width - 1, 21 - 2*height - 3);
        DrawKey(1, 3);
        Graphics::Render(skeleton, 4, 29);
    }
    else {
        DrawBoard(50 - width - 1, 21 - 2*height - 3);
        DrawKey(1, 3);
        Graphics::Render(skeleton, 4, 29);

        if (turn > 0) {
            Graphics::DrawText("Turn: " + to_string(turn), 1, 1);

            // Render the controls box
            string controls = "move [u]p   move [l]eft   move [d]own   move [r]ight   [a]ttack ";
            controls += GetCursorPos();
            Graphics::DrawText(controls, 1, 48);
        }
        else
            Graphics::DrawText("[c]ontinue", 1, 48);
    }
}

void SkeletonShip::Input(char input) {
    if (input == '9')
        turn = -2;
    else if (input == '0')
        turn = -3;

    if (turn == -1) {
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
    else if (turn == 0) {
        switch(input) {
            case 'u':
                cursorY = max(cursorY - 1, 0);
                game->Log("You moved your cursor to " + GetCursorPos());
                break;
            case 'l':
                cursorX = max(cursorX - 1, 0);
                game->Log("You moved your cursor to " + GetCursorPos());
                break;
            case 'd':
                cursorY = min(cursorY + 1, height - 1);
                game->Log("You moved your cursor to " + GetCursorPos());
                break;
            case 'r':
                cursorX = min(cursorX + 1, width - 1);
                game->Log("You moved your cursor to " + GetCursorPos());
                break;
            case 'o':
                isVertical = !isVertical;
                if (isVertical)
                    game->Log("Placing ships vertically");
                else
                    game->Log("Placing ships horizontally");
                break;
            case 'p':
                if (PlaceShip(cursorX, cursorY, isVertical, currentShip, boardPlayer)) {
                    switch (currentShip) {
                        case 'C': currentShip = 'B'; break;
                        case 'B': currentShip = 'S'; break;
                        case 'S': currentShip = 'c'; break;
                        case 'c': currentShip = 'D'; break;
                        case 'D':
                            SkeletonSetup();
                            turn++;
                            break;
                    }
                }
                else {
                    game->Log("Can't place a ship there!");
                }
                break;
        }
    }
    else {
        switch(input) {
            case 'u':
                cursorY = max(cursorY - 1, 0);
                game->Log("You moved your cursor to " + GetCursorPos());
                break;
            case 'l':
                cursorX = max(cursorX - 1, 0);
                game->Log("You moved your cursor to " + GetCursorPos());
                break;
            case 'd':
                cursorY = min(cursorY + 1, height - 1);
                game->Log("You moved your cursor to " + GetCursorPos());
                break;
            case 'r':
                cursorX = min(cursorX + 1, width - 1);
                game->Log("You moved your cursor to " + GetCursorPos());
                break;
            case 'a':
                if (Attack(cursorX, cursorY, boardSkeleton))
                    game->Log("You struck a ship at " + GetCursorPos() + "!");
                else
                    game->Log("Your shot at " + GetCursorPos() + " was a dud...");
                if (!CheckWinCon())
                    SkeletonTurn();
                break;
        }
    }
}

bool SkeletonShip::PlaceShip(int x, int y, bool isVertical, char type, char** board) {
    int length;
    switch (type) {
        case 'C': length = 6; break;
        case 'B': length = 5; break;
        case 'S': length = 4; break;
        case 'c': length = 4; break;
        case 'D': length = 3; break;
        default: return false;
    }

    // Check boundaries
    if (isVertical && y + length > height)
        return false;
    else if (!isVertical && y + length > width)
        return false;

    // Check collision
    for (int i = 0; i < length; i++) {
        if (isVertical && board[y+i][x] != '.')
            return false;
        else if (!isVertical && board[y][x+i] != '.')
            return false;
    }

    for (int i = 0; i < length; i++) {
        if (isVertical)
            board[y+i][x] = type;
        else
            board[y][x+i] = type;
    }

    return true;
}

bool SkeletonShip::Attack(int x, int y, char** board) {
    if (board[y][x] == '.' || board[y][x] == 'X' || board[y][x] == '_') {
        if (board[y][x] == '.')
            board[y][x] = '_';
        return false;
    }

    char marker = board[y][x];
    board[y][x] = 'X';

    // Check if ship was sunk
    bool foundShip = false;
    for (int i = 0; i < width*height; i++)
        if (board[i / width][i % width] == marker) {
            foundShip = true;
            break;
        }

    if (!foundShip) {
        if (board == boardPlayer)
            game->Log("Your " + GetShipName(marker) + " has been sunk!");
        else
            game->Log("You sunk the skeleton's " + GetShipName(marker));
    }

    return true;
}

int SkeletonShip::CheckWinCon() {
    switch (GetWinCon()) {
        default:
        case 0:
            return 0;
        case 1:
            game->Log("\"Brilliant! You have the skills to command the greatest of navies!\"");
            turn = -2;
            return 1;
        case 2:
            game->Log("\"Fret not! It is no wonder you succumbed to my wit...\"");
            turn = -3;
            return 2;
    }
}

int SkeletonShip::GetWinCon() {
    // Check player board
    {
        bool foundShips = false;
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                if (boardPlayer[y][x] != '.' && boardPlayer[y][x] != 'X' && boardPlayer[y][x] != '_')
                    foundShips = true;
        if (!foundShips)
            return 2;
    }
    {
        bool foundShips = false;
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                if (boardSkeleton[y][x] != '.' && boardSkeleton[y][x] != 'X' && boardSkeleton[y][x] != '_')
                    foundShips = true;
        if (!foundShips)
            return 1;
    }
    return 0;
}

void SkeletonShip::SkeletonSetup() {
    while (!PlaceShip(rand() % (width - 5), rand() % (height - 5), rand() % 2, 'C', boardSkeleton));
    while (!PlaceShip(rand() % (width - 4), rand() % (height - 4), rand() % 2, 'B', boardSkeleton));
    while (!PlaceShip(rand() % (width - 3), rand() % (height - 3), rand() % 2, 'S', boardSkeleton));
    while (!PlaceShip(rand() % (width - 3), rand() % (height - 3), rand() % 2, 'c', boardSkeleton));
    while (!PlaceShip(rand() % (width - 2), rand() % (height - 2), rand() % 2, 'D', boardSkeleton));
}

void SkeletonShip::DrawBoard(int _x, int _y) {
    // Draw frame
    Graphics::DrawBox(_x, _y, _x + 2*width + 2, _y + 2*height + 2);
    Graphics::DrawBox(_x, _y + 2*height + 3, _x + 2*width + 2, _y + 4*height + 5);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Draw markers
            Graphics::DrawPixel(boardPlayer[y][x], _x + 2 + 2*x, _y + 2 + 2*y);
            if (boardSkeleton[y][x] != '.' && boardSkeleton[y][x] != '_' && boardSkeleton[y][x] != 'X')
                Graphics::DrawPixel('.', _x + 2 + 2*x, _y + 5 + 2*y + 2*height);
            else
                Graphics::DrawPixel(boardSkeleton[y][x], _x + 2 + 2*x, _y + 5 + 2*y + 2*height);
        }
    }

    // Draw labels
    Graphics::DrawText("Player", _x + 2*width + 3, _y + 2, true);
    Graphics::DrawText("Skeleton", _x - 1, _y + 2*height + 5, true);
    for (int y = 0; y < height; y++) {
        Graphics::DrawText(to_string((y + 1) % 10), _x - 1, _y + 2*y + 2);
        Graphics::DrawText(to_string((y + 1) % 10), _x + 2*width + 3, _y + 2*y + 2*height + 5);
    }
    for (int x = 0; x < width; x++) {
        Graphics::DrawText(string(1, 'A'+x), _x + 2 + 2*x, _y - 1);
        Graphics::DrawText(string(1, 'A'+x), _x + 2 + 2*x, _y + 4*height + 6);
    }
}

void SkeletonShip::SkeletonTurn() {
    if (rand() % 10 < difficulty) {
        int moveCount = 0;
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                if (boardPlayer[y][x] != '.' && boardPlayer[y][x] != 'X' && boardPlayer[y][x] != '_')
                    moveCount++;

        int move = rand() % moveCount;
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                if (boardPlayer[y][x] != '.' && boardPlayer[y][x] != 'X' && boardPlayer[y][x] != '_')
                    if (move-- == 0) {
                        Attack(x, y, boardPlayer);
                        if (!CheckWinCon())
                            turn++;
                        return;
                    }
    }
    else {
        bool hasAttacked = 0;
        do {
            int x = rand() % width;
            int y = rand() % height;

            if (boardPlayer[y][x] != 'X' && boardPlayer[y][x] != '_') {
                Attack(x, y, boardPlayer);
                CheckWinCon();
                hasAttacked = true;
            }
        }
        while (!hasAttacked);
    }
}

void SkeletonShip::DrawKey(int x, int y) {
    Graphics::DrawText("Carrier:    CCCCCC", x, y);
    Graphics::DrawText("Battleship: BBBBB", x, y + 1);
    Graphics::DrawText("Submarine:  SSSS", x, y + 2);
    Graphics::DrawText("Cruiser:    cccc", x, y + 3);
    Graphics::DrawText("Destroyer:  DDD", x, y + 4);

    Graphics::DrawText("X: Hit", x, y + 6);
    Graphics::DrawText("_: Dud", x, y + 7);
}

string SkeletonShip::GetCursorPos(int x, int y) {
    if (x == -1 && y == -1)
        return string(1, 'A'+cursorX) + to_string(cursorY + 1);
    return string(1, 'A'+x) + to_string(y + 1);
}

string SkeletonShip::GetShipName(char type) {
    switch (type) {
        case 'C': return "carrier";
        case 'B': return "battleship";
        case 'S': return "submarine";
        case 'c': return "cruiser";
        case 'D': return "destroyer";
    }
    return "ERROR";
}
