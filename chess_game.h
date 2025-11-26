#pragma once
#include <utility>

// create enum for piece types and colors
// enum for chess pieces:
enum PieceType {
    WHITE_PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING
};

enum class GameStatus {
    ONGOING,
    WHITE_WIN,
    BLACK_WIN,
    DRAW
};

class ChessGame {
public:
    ChessGame();
    ~ChessGame();

    PieceType getPieceAt(int row, int col);

    // make a move from (srcRow, srcCol) to (destRow, destCol)
    bool makeMove(int srcRow, int srcCol, int destRow, int destCol);

    //query the state of the game
    GameStatus getGameStatus() const;

    bool isWhiteTurn() const;
private:
    // for the internal representation of the chessboard
    PieceType board[8][8];
    bool whiteTurn;
    GameStatus status;

    bool inBounds(int r, int c) const;
    bool isEmpty(PieceType p) const;
    bool isWhitePiece(PieceType p) const;
    bool isBlackPiece(PieceType p) const;
    bool sameColor(PieceType a, PieceType b) const;

    bool isPathClear(int sr, int sc, int dr, int dc) const;
    bool isPseudoLegalBasic(int sr, int sc, int dr, int dc) const;

    // for promotion
    PieceType whiteQueen() const { return WHITE_QUEEN; }
    PieceType blackQueen() const { return BLACK_QUEEN; }

    std::pair<int, int> findKing(bool white) const;
    bool isSquareAttackedBy(int r, int c, bool byWhite) const;

    bool whiteKingMoved = false;
    bool blackKingMoved = false;
    bool whiteRookLeftMoved = false;
    bool whiteRookRightMoved = false;
    bool blackRookLeftMoved = false;
    bool blackRookRightMoved = false;

    int enPassantCol = -1;
    int enPassantRow = -1;

    bool hasAnyLegalMoves(bool white);

    int halfMoveClock = 0;

    bool hasInsufficientMaterial() const;
};