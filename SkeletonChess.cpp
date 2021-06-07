#include "SkeletonChess.h"

#include <cmath>
#include "Game.h"

SkeletonChess::SkeletonChess(Game* game, int _difficulty)
    : game(game)
    , difficulty(_difficulty) {
    cursorX = 0;
    cursorY = 0;
    selectX = 0;
    selectY = 0;

    board = new char*[8];
    for (int y = 0; y < 8; y++) {
        board[y] = new char[8];
        for (int x = 0; x < 8; x++)
            board[y][x] = '-';
    }

    switch (difficulty) {
        case 0:
            ImportPartialFEN("8/8/2KB4/7B/8/8/5n2/4kr2");
            solution = "8/8/2K5/7B/1B6/8/5n2/4kr2";
            break;
        case 1:
            ImportPartialFEN("5qk1/3R4/6pP/6Q1/8/8/1B6/8");
            solution = "5qk1/3R3P/6p1/6Q1/8/8/1B6/8";
            break;
        case 2:
            ImportPartialFEN("r1bq1k1r/pppp3p/1b1P1pnn/6B1/2B5/5N1P/PP2QPP1/RN2K2R");
            solution = "r1bq1k1r/pppp3p/1b1P1pnB/8/2B5/5N1P/PP2QPP1/RN2K2R";
    }

    SelectNext();

    skeletonClose = Sprite("assets/skeletonClose.txt");
    skeleton = Sprite("assets/skeleton.txt");
}

void SkeletonChess::Loop() {
    if (turn == 0) {
        // Render the controls box
        Graphics::DrawText("[c]ontinue", 1, 48);

        Graphics::Render(skeletonClose, 18, 9);

        game->Log("\"To pass you must solve this mate-in-one!\"");
    }
    else {
        DrawBoard(50 - (3*8 + 3)/2, 21 - (2*8 + 2)/2);
        Graphics::Render(skeleton, 4, 29);

        if (turn > 0) {
            if (inSelectMode) {
                // Render the controls box
                string controls = "[n]ext piece   [p]revious piece   [s]elect ";
                controls += GetPieceName(board[selectY][selectX]);
                controls += " at " + GetCursorPos(selectX, selectY);
                Graphics::DrawText(controls, 1, 48);
            }
            else {
                // Render the controls box
                string controls = "move [u]p   move [l]eft   move [d]own   move [r]ight   ";
                if (CheckValidity(selectX, selectY, cursorX, cursorY)) {
                    if (IdentifyPiece(cursorX, cursorY) == 2) {
                        controls += "[c]apture ";
                        controls += GetPieceName(board[cursorY][cursorX]);
                        controls += " at " + GetCursorPos(cursorX, cursorY);
                    }
                    else {
                        controls += "[m]ove ";
                        controls += GetPieceName(board[selectY][selectX]);
                        controls += " at " + GetCursorPos(cursorX, cursorY);
                    }
                }
                controls += "   ca[n]cel";
                Graphics::DrawText(controls, 1, 48);
            }
        }
        else
            Graphics::DrawText("[c]ontinue", 1, 48);
    }
}

void SkeletonChess::Input(char input) {
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
        if (inSelectMode) {
            switch (input) {
                case 'n':
                    SelectNext();
                    break;
                case 'p':
                    SelectPrev();
                    break;
                case 's':
                    inSelectMode = false;
                    cursorX = selectX;
                    cursorY = selectY;
                    break;
            }
        }
        else {
            switch (input) {
                case 'u':
                    cursorY = max(cursorY - 1, 0);
                    game->Log("You moved your cursor to " + GetCursorPos(cursorX, cursorY));
                    break;
                case 'l':
                    cursorX = max(cursorX - 1, 0);
                    game->Log("You moved your cursor to " + GetCursorPos(cursorX, cursorY));
                    break;
                case 'd':
                    cursorY = min(cursorY + 1, 7);
                    game->Log("You moved your cursor to " + GetCursorPos(cursorX, cursorY));
                    break;
                case 'r':
                    cursorX = min(cursorX + 1, 7);
                    game->Log("You moved your cursor to " + GetCursorPos(cursorX, cursorY));
                    break;
                case 'c':
                case 'm':
                    if (CheckValidity(selectX, selectY, cursorX, cursorY)) {
                        board[cursorY][cursorX] = board[selectY][selectX];
                        board[selectY][selectX] = '-';
                        CheckWinCon();
                    }
                    break;
                case 'n':
                    inSelectMode = true;
                    break;
            }
        }
    }
}

void SkeletonChess::SelectNext() {
    for (int i = 8*selectY + selectX + 1; i < 8*8; i++) {
        if (IdentifyPiece(i % 8, i / 8) == 1) {
            selectX = i % 8;
            selectY = i / 8;
            break;
        }
        if (i == 8*8 - 1)
            i = 0;
    }
}

void SkeletonChess::SelectPrev() {
    for (int i = 8*selectY + selectX - 1; i >= 0; i--) {
        if (IdentifyPiece(i % 8, i / 8) == 1) {
            selectX = i % 8;
            selectY = i / 8;
            break;
        }
        if (i == 0)
            i = 8*8 - 1;
    }
}

bool SkeletonChess::CheckValidity(int x1, int y1, int x2, int y2) {
    char piece = board[y1][x1];
    bool isWhite = IdentifyPiece(x1, y1) == 1;

    // Check if piece is moving
    if (x1 == x2 && y1 == y2)
        return false;

    // Check if final position is ally
    if ((isWhite && IdentifyPiece(x2, y2) == 1) ||
        (!isWhite && IdentifyPiece(x2, y2) == 2))
        return false;

    int dirX, dirY;

    switch (piece) {
        case 'p':
        case 'P':
            // Check for pawn movement
            if (isWhite && y2 - y1 == -1) {
                if (x1 == x2)
                    return true;
                else if (IdentifyPiece(x2, y2) == 2 && abs(x2 - x1) == 1)
                    return true;
            }
            if (!isWhite && y2 - y1 == 1) {
                if (x1 == x2)
                    return true;
                else if (IdentifyPiece(x2, y2) == 1 && abs(x2 - x1) == 1)
                    return true;
            }

            return false;
        case 'k':
        case 'K':
            // Check for king movement
            if (pow(x2 - x1, 2) + pow(y2 - y1, 2) <= 2)
                return true;

            return false;
        case 'q':
        case 'Q':
            // Check for queen movement
            if (!((x2 - x1 == y2 - y1) ||
                (x2 - x1 == y1 - y2) ||
                (x1 - x2 == y2 - y1) ||
                (x1 - x2 == y1 - y2)) ||
                (x1 != x2 && y1 != y2))
                return false;

            // Check path collision
            dirX = x2 == x1 ? 0 : (x2 > x1 ? 1 : -1);
            dirY = y2 == y1 ? 0 : (y2 > y1 ? 1 : -1);
            for (int i = 1; i < abs((x2 - x1) + (y2 - y1)); i++)
                if (board[y1 + dirY*i][x1 + dirX*i] != '-')
                    return false;

            return false;
        case 'b':
        case 'B':
            // Check for bishop movement
            if (!((x2 - x1 == y2 - y1) ||
                (x2 - x1 == y1 - y2) ||
                (x1 - x2 == y2 - y1) ||
                (x1 - x2 == y1 - y2)))
                return false;

            return true;
        case 'n':
        case 'N':
            // Check for knight movement
            if ((x2 - x1 == 2 && y2 - y1 == 1) ||
                (x2 - x1 == 2 && y2 - y1 == -1) ||
                (x2 - x1 == -2 && y2 - y1 == 1) ||
                (x2 - x1 == -2 && y2 - y1 == -1) ||
                (x2 - x1 == 1 && y2 - y1 == 2) ||
                (x2 - x1 == 1 && y2 - y1 == -2) ||
                (x2 - x1 == -1 && y2 - y1 == 2) ||
                (x2 - x1 == -1 && y2 - y1 == -2))
                return true;

            return false;
        case 'r':
        case 'R':
            // Check for rook movement
            if (x1 != x2 && y1 != y2)
                return false;

            // Check path collision
            dirX = x2 == x1 ? 0 : (x2 > x1 ? 1 : -1);
            dirY = y2 == y1 ? 0 : (y2 > y1 ? 1 : -1);
            for (int i = 1; i < abs((x2 - x1) + (y2 - y1)); i++)
                if (board[y1 + dirY*i][x1 + dirX*i] != '-')
                    return false;

            return true;
    }

    return false;
}

int SkeletonChess::IdentifyPiece(int x, int y) {
    if (board[y][x] >= 'A' && board[y][x] <= 'Z')
        return 1;
    if (board[y][x] >= 'a' && board[y][x] <= 'z')
        return 2;
    return 0;
}

int SkeletonChess::CheckWinCon() {
    if (ExportPartialFEN() == solution) {
        game->Log("\"You hold quite a clever bone!\"");
        turn = -2;
        return 1;
    }
    game->Log("\"What's all that flesh in your skull for?\"");
    turn = -3;
    return 2;
}

void SkeletonChess::ImportPartialFEN(string fen) {
    int i = 0;
    for (char fenChar : fen) {
        if (fenChar >= '1' && fenChar <= '8')
            i += fenChar - '0';
        else if (fenChar != '/') {
            board[i / 8][i % 8] = fenChar;
            i++;
        }
    }
}

string SkeletonChess::ExportPartialFEN() {
    string fen = "";
    int emptyCount = 0;
    for (int i = 0; i < 8*8; i++) {
        int x = i % 8;
        int y = i / 8;
        if (x == 0 && i != 0) {
            if (emptyCount > 0) {
                fen += '0' + emptyCount;
                emptyCount = 0;
            }
            fen += '/';
        }
        if (IdentifyPiece(x, y)) {
            if (emptyCount > 0) {
                fen += '0' + emptyCount;
                emptyCount = 0;
            }
            fen += board[y][x];
        }
        else
            emptyCount++;
    }
    if (emptyCount > 0) {
        fen += '0' + emptyCount;
        emptyCount = 0;
    }

    return fen;
}

void SkeletonChess::DrawBoard(int _x, int _y) {
    Graphics::DrawBox(_x, _y, _x + 3*8 + 3, _y + 2*8 + 2);
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            // Draw pieces
            Graphics::DrawPixel(board[y][x], _x + 3 + 3*x, _y + 2 + 2*y);
        }
    }

    // Draw labels
    for (int y = 0; y < 8; y++)
        Graphics::DrawPixel('1'+y, _x - 1, _y + 2 + 2*y);
    for (int x = 0; x < 8; x++)
        Graphics::DrawPixel('A'+x, _x + 3 + 3*x, _y - 1);
}

string SkeletonChess::GetCursorPos(int x, int y) {
    return string(1, 'A'+x) + to_string(y + 1);
}

string SkeletonChess::GetPieceName(char type) {
    switch (type) {
        case 'r': return "black rook";
        case 'n': return "black knight";
        case 'b': return "black bishop";
        case 'q': return "black queen";
        case 'k': return "black king";
        case 'p': return "black pawn";
        case 'R': return "white rook";
        case 'N': return "white knight";
        case 'B': return "white bishop";
        case 'Q': return "white queen";
        case 'K': return "white king";
        case 'P': return "white pawn";
        default: return "ERROR";
    }
}
