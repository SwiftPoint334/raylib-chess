#pragma once
#include "raylib.h"
#include "chess_game.h"

class ChessAnalysisProgram {
public:
    ChessAnalysisProgram();
    ~ChessAnalysisProgram();
    void run();
private:
    Texture2D boardTexture;
    // textures for all black and white pieces
    Texture2D whiteKnightTexture;
    Texture2D whiteBishopTexture;
    Texture2D whiteRookTexture;
    Texture2D whiteQueenTexture;
    Texture2D whiteKingTexture;
    Texture2D whitePawnTexture;
    Texture2D blackKnightTexture;
    Texture2D blackBishopTexture;
    Texture2D blackRookTexture;
    Texture2D blackQueenTexture;
    Texture2D blackKingTexture;
    Texture2D blackPawnTexture;

    // current game state:
    ChessGame currentPosition;

    void updateGame();
    void renderBoard();
    void renderUI();
    void renderPieces();
    void renderGame();
    void loadAllTextures();
    void unloadAllTextures();
};