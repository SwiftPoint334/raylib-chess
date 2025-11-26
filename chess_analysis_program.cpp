#include <raylib.h>
#include "chess_analysis_program.h"
#include <stdexcept>

ChessAnalysisProgram::ChessAnalysisProgram() {
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "Chess analysis");
    SetTargetFPS(120);

    ToggleFullscreen();

    try {
        loadAllTextures();
    } catch (const std::runtime_error& e) {
        unloadAllTextures();
        CloseWindow();
    }

    currentPosition = ChessGame();
}

ChessAnalysisProgram::~ChessAnalysisProgram() {
    unloadAllTextures();
    CloseWindow();
}

void ChessAnalysisProgram::loadAllTextures() {
    boardTexture = LoadTexture("../images/woodenboard.png");
    if (boardTexture.id == 0) {
        throw std::runtime_error("Failed to load woodenboard.png");
    }

    // load pieces
    whiteRookTexture = LoadTexture("../images/wr.png");
    if (whiteRookTexture.id == 0) {
        throw std::runtime_error("Failed to load wr.png");
    }
    whiteBishopTexture = LoadTexture("../images/wb.png");
    if (whiteBishopTexture.id == 0) {
        throw std::runtime_error("Failed to load wb.png");
    }
    whiteKnightTexture = LoadTexture("../images/wn.png");
    if (whiteKnightTexture.id == 0) {
        throw std::runtime_error("Failed to load wn.png");
    }
    whiteQueenTexture = LoadTexture("../images/wq.png");
    if (whiteQueenTexture.id == 0) {
        throw std::runtime_error("Failed to load wq.png");
    }
    whiteKingTexture = LoadTexture("../images/wk.png");
    if (whiteKingTexture.id == 0) {
        throw std::runtime_error("Failed to load wk.png");
    }
    whitePawnTexture = LoadTexture("../images/wp.png");
    if (whitePawnTexture.id == 0) {
        throw std::runtime_error("Failed to load wp.png");
    }
    blackRookTexture = LoadTexture("../images/br.png");
    if (blackRookTexture.id == 0) {
        throw std::runtime_error("Failed to load br.png");
    }
    blackBishopTexture = LoadTexture("../images/bb.png");
    if (blackBishopTexture.id == 0) {
        throw std::runtime_error("Failed to load bb.png");
    }
    blackKnightTexture = LoadTexture("../images/bn.png");
    if (blackKnightTexture.id == 0) {
        throw std::runtime_error("Failed to load bn.png");
    }
    blackQueenTexture = LoadTexture("../images/bq.png");
    if (blackQueenTexture.id == 0) {
        throw std::runtime_error("Failed to load bq.png");
    }
    blackKingTexture = LoadTexture("../images/bk.png");
    if (blackKingTexture.id == 0) {
        throw std::runtime_error("Failed to load bk.png");
    }
    blackPawnTexture = LoadTexture("../images/bp.png");
    if (blackPawnTexture.id == 0) {
        throw std::runtime_error("Failed to load bp.png");
    }
}

void ChessAnalysisProgram::unloadAllTextures() {
    if (boardTexture.id != 0) UnloadTexture(boardTexture);
    if (whiteRookTexture.id != 0) UnloadTexture(whiteRookTexture);
    if (whiteBishopTexture.id != 0) UnloadTexture(whiteBishopTexture);
    if (whiteKnightTexture.id != 0) UnloadTexture(whiteKnightTexture);
    if (whiteQueenTexture.id != 0) UnloadTexture(whiteQueenTexture);
    if (whiteKingTexture.id != 0) UnloadTexture(whiteKingTexture);
    if (whitePawnTexture.id != 0) UnloadTexture(whitePawnTexture);
    if (blackRookTexture.id != 0) UnloadTexture(blackRookTexture);
    if (blackBishopTexture.id != 0) UnloadTexture(blackBishopTexture);
    if (blackKnightTexture.id != 0) UnloadTexture(blackKnightTexture);
    if (blackQueenTexture.id != 0) UnloadTexture(blackQueenTexture);
    if (blackKingTexture.id != 0) UnloadTexture(blackKingTexture);
    if (blackPawnTexture.id != 0) UnloadTexture(blackPawnTexture);
}

void ChessAnalysisProgram::updateGame() {
    // handle input and update game state here
    static bool dragging = false;
    static int selectedRow = -1;
    static int selectedCol = -1;

    float tileSize = boardTexture.width * 0.675f / 8.0f;
    Vector2 mouse = GetMousePosition();

    int hoveredCol = mouse.x / tileSize;
    int hoveredRow = mouse.y / tileSize;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (hoveredRow >= 0 && hoveredRow < 8 && hoveredCol >= 0 && hoveredCol < 8) {
            PieceType piece = currentPosition.getPieceAt(hoveredRow, hoveredCol);
            if (piece != static_cast<PieceType>(-1)) {
                dragging = true;
                selectedRow = hoveredRow;
                selectedCol = hoveredCol;
            }
        }
    }

    if (dragging && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (hoveredRow >= 0 && hoveredRow < 8 && hoveredCol >= 0 && hoveredCol < 8) {
            currentPosition.makeMove(selectedRow, selectedCol, hoveredRow, hoveredCol);
        }
        dragging = false;
        selectedRow = -1;
        selectedCol = -1;
    }
}

void ChessAnalysisProgram::renderGame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    renderBoard();
    renderPieces();
    renderUI();

    EndDrawing();
}

void ChessAnalysisProgram::renderUI() {

}

void ChessAnalysisProgram::renderBoard() {
    DrawTextureEx(this->boardTexture, {0.0f, 0.0f}, 0.0f, 0.675f, WHITE);
}

void ChessAnalysisProgram::renderPieces() {
    // render all chess pieces here
    // go through all squares on the board and draw the appropriate piece
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            PieceType piece = currentPosition.getPieceAt(row, col);
            Texture2D textureToDraw;
            switch (piece) {
                case WHITE_PAWN:
                    textureToDraw = whitePawnTexture;
                    break;
                case WHITE_KNIGHT:
                    textureToDraw = whiteKnightTexture;
                    break;
                case WHITE_BISHOP:
                    textureToDraw = whiteBishopTexture;
                    break;
                case WHITE_ROOK:
                    textureToDraw = whiteRookTexture;
                    break;
                case WHITE_QUEEN:
                    textureToDraw = whiteQueenTexture;
                    break;
                case WHITE_KING:
                    textureToDraw = whiteKingTexture;
                    break;
                case BLACK_PAWN:
                    textureToDraw = blackPawnTexture;
                    break;
                case BLACK_KNIGHT:
                    textureToDraw = blackKnightTexture;
                    break;
                case BLACK_BISHOP:
                    textureToDraw = blackBishopTexture;
                    break;
                case BLACK_ROOK:
                    textureToDraw = blackRookTexture;
                    break;
                case BLACK_QUEEN:
                    textureToDraw = blackQueenTexture;
                    break;
                case BLACK_KING:
                    textureToDraw = blackKingTexture;
                    break;
                default:
                    continue; // empty square
            }
            // calculate position to draw the piece
            float tileSize = boardTexture.width * 0.675f / 8.0f;
            DrawTextureEx(textureToDraw, {col * tileSize, row * tileSize}, 0.0f, tileSize / textureToDraw.width, WHITE);
        }
    }
}

void ChessAnalysisProgram::run() {

    while (!WindowShouldClose()) {
        updateGame();
        renderGame();
    }
    CloseWindow();
}