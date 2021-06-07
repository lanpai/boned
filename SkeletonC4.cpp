#include "SkeletonC4.h"

#include "Game.h"

SkeletonC4::SkeletonC4(Game* game, int _difficulty)
    : game(game)
    , difficulty(_difficulty) {
    width = 8 - difficulty;

    cursor = 0;

    board = new int*[C4_HEIGHT];
    for (int y = 0; y < C4_HEIGHT; y++) {
        board[y] = new int[width];
        for (int x = 0; x < width; x++)
            board[y][x] = 0;
    }

    skeletonClose = Sprite("assets/skeletonClose.txt");
    skeleton = Sprite("assets/skeleton.txt");
    skeletonPiece = Sprite("assets/c4skeleton.txt");
    playerPiece = Sprite("assets/c4player.txt");
}

void SkeletonC4::Loop() {
    if (turn == 0) {
        // Render the controls box
        Graphics::DrawText("[c]ontinue", 1, 48);

        Graphics::Render(skeletonClose, 18, 9);

        game->Log("\"I stand undefeated in my connect four, human!\"");
    }
    else {
        DrawBoard(50 - (6*width)/2, 21 - (5*C4_HEIGHT)/2);
        Graphics::Render(skeleton, 4, 29);

        if (turn > 0) {
            Graphics::DrawText("Turn: " + to_string(turn), 1, 1);

            // Render the controls box
            string controls = "move [l]eft   move [r]ight   [p]lace a piece at ";
            controls += to_string(cursor + 1);
            Graphics::DrawText(controls, 1, 48);
        }
        else
            Graphics::DrawText("[c]ontinue", 1 ,48);
    }
}

void SkeletonC4::Input(char input) {
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
            case 'l':
                cursor = max(cursor - 1, 0);
                game->Log("You moved your cursor to " + to_string(cursor + 1));
                break;
            case 'r':
                cursor = min(cursor + 1, width - 1);
                game->Log("You moved your cursor to " + to_string(cursor + 1));
                break;
            case 'p':
                if (board[0][cursor] == 0) {
                    PlacePiece(cursor, 1);
                    game->Log("You placed a piece at " + to_string(cursor + 1));
                    if (!CheckWinCon())
                        SkeletonTurn();
                }
                else {
                    game->Log("Column " + to_string(cursor + 1) + " is full!");
                }
                break;
        }
    }
}

void SkeletonC4::PlacePiece(int x, int marker) {
    if (board[0][cursor] == 0)
        for (int y = C4_HEIGHT - 1; y >= 0; y--)
            if (board[y][x] == 0) {
                board[y][x] = marker;
                break;
            }
}

int SkeletonC4::CheckWinCon() {
    switch (GetWinCon()) {
        default:
        case 0:
            return 0;
        case 1:
            game->Log("\"A true display of talent! Remarkable!\"");
            turn = -2;
            return 1;
        case 2:
            game->Log("\"Yet another victory under my bones!\"");
            turn = -3;
            return 2;
        case 3:
            game->Log("\"A tie is quite rare in this game of ours! Remarkable!\"");
            ClearBoard();
            return 3;
    }
}

int SkeletonC4::GetWinCon() {
    // Horizontal checks
    for (int y = 0; y < C4_HEIGHT; y++) {
        for (int x = 0; x < width; x++) {
            if (board[y][x]) {
                int marker = board[y][x];

                // Horizontal check
                if (x + 4 <= width) {
                    bool meetsWinCon = true;
                    for (int xPrime = x; xPrime < x + 4; xPrime++)
                        if (board[y][xPrime] != marker) {
                            meetsWinCon = false;
                            break;
                        }
                    if (meetsWinCon)
                        return marker;
                }

                // Vertical check
                if (y + 4 <= C4_HEIGHT) {
                    bool meetsWinCon = true;
                    for (int yPrime = y; yPrime < y + 4; yPrime++)
                        if (board[yPrime][x] != marker) {
                            meetsWinCon = false;
                            break;
                        }
                    if (meetsWinCon)
                        return marker;
                }

                // Diagonal check
                if (x + 4 <= width && y + 4 <= C4_HEIGHT) {
                    bool meetsWinCon = true;
                    for (int i = 0; i < 4; i++)
                        if (board[y + i][x + i] != marker) {
                            meetsWinCon = false;
                            break;
                        }
                    if (meetsWinCon)
                        return marker;
                }
                if (x - 3 >= 0 && y + 4 <= C4_HEIGHT) {
                    bool meetsWinCon = true;
                    for (int i = 0; i < 4; i++)
                        if (board[y + i][x - i] != marker) {
                            meetsWinCon = false;
                            break;
                        }
                    if (meetsWinCon)
                        return marker;
                }
            }
        }
    }

    // Tie check
    bool hasEmpty = false;
    for (int y = 0; y < C4_HEIGHT; y++)
        for (int x = 0; x < width; x++)
            if (board[y][x] == 0) {
                hasEmpty = true;
                break;
            }
    if (!hasEmpty)
        return 3;

    return 0;
}

void SkeletonC4::SkeletonTurn() {
    int moveCount = 0;
    for (int x = 0; x < width; x++)
        if (board[0][x] == 0)
            moveCount++;

    int move = rand() % moveCount;
    for (int x = 0; x < width; x++)
        if (board[0][x] == 0)
            if (move-- == 0) {
                PlacePiece(x, 2);
                if (!CheckWinCon())
                    turn++;
                return;
            }
}

void SkeletonC4::DrawBoard(int _x, int _y) {
    Graphics::DrawLine(_x, _y + 5*C4_HEIGHT, 6*width + 1, false);
    for (int y = 0; y < C4_HEIGHT; y++) {
        Graphics::DrawLine(_x, _y + 5*y, 5, true);
        for (int x = 0; x < width; x++) {
            // Draw frame
            Graphics::DrawLine(_x + 6*x + 6, _y + 5*y, 5, true);

            // Draw markers
            switch (board[y][x]) {
                case 0:
                    break;
                case 1:
                    Graphics::Render(playerPiece, _x + 6*x + 1, _y + 5*y);
                    break;
                case 2:
                    Graphics::Render(skeletonPiece, _x + 6*x + 1, _y + 5*y);
                    break;
            }
        }
    }

    // Draw labels
    for (int x = 0; x < width; x++)
        Graphics::DrawText(string(1,'1'+x), _x + 6*x + 3, _y - 1);
}

void SkeletonC4::ClearBoard() {
    for (int y = 0; y < C4_HEIGHT; y++)
        for (int x = 0; x < width; x++)
            board[y][x] = 0;
}
