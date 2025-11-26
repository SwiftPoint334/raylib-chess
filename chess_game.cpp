#include "chess_game.h"
#include <cstdlib>

ChessGame::ChessGame() {

    whiteTurn = true;
    status = GameStatus::ONGOING;

    // clear board
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            board[row][col] = static_cast<PieceType>(-1); // have to use this to represent empty square, random pieces were showing
        }
    }
    // pawns
    for (int c = 0; c < 8; ++c) {
        board[1][c] = BLACK_PAWN; // black pawns (top)
        board[6][c] = WHITE_PAWN; // white pawns (bottom)
    }

    // black back rank (top)
    board[0][0] = BLACK_ROOK;   board[0][7] = BLACK_ROOK;
    board[0][1] = BLACK_KNIGHT; board[0][6] = BLACK_KNIGHT;
    board[0][2] = BLACK_BISHOP; board[0][5] = BLACK_BISHOP;
    board[0][3] = BLACK_QUEEN;  board[0][4] = BLACK_KING;

    // white back rank (bottom)
    board[7][0] = WHITE_ROOK;   board[7][7] = WHITE_ROOK;
    board[7][1] = WHITE_KNIGHT; board[7][6] = WHITE_KNIGHT;
    board[7][2] = WHITE_BISHOP; board[7][5] = WHITE_BISHOP;
    board[7][3] = WHITE_QUEEN;  board[7][4] = WHITE_KING;
}

ChessGame::~ChessGame() {
    // destructor
}

bool ChessGame::inBounds(int r, int c) const {
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}

bool ChessGame::isEmpty(PieceType p) const {
    return p == static_cast<PieceType>(-1);
}

bool ChessGame::isWhitePiece(PieceType p) const {
    switch (p) {
        case WHITE_PAWN:
        case WHITE_KNIGHT:
        case WHITE_BISHOP:
        case WHITE_ROOK:
        case WHITE_QUEEN:
        case WHITE_KING:
            return true;
        default:
            return false;
    }
}

bool ChessGame::isBlackPiece(PieceType p) const {
    switch (p) {
        case BLACK_PAWN:
        case BLACK_KNIGHT:
        case BLACK_BISHOP:
        case BLACK_ROOK:
        case BLACK_QUEEN:
        case BLACK_KING:
            return true;
        default:
            return false;
    }
}

bool ChessGame::sameColor(PieceType a, PieceType b) const {
    if (isEmpty(a) || isEmpty(b)) {
        return false;
    }
    return (isWhitePiece(a) && isWhitePiece(b)) || (isBlackPiece(a) && isBlackPiece(b));
}

bool ChessGame::isPathClear(int sr, int sc, int dr, int dc) const {
    int rstep = (dr == sr ? 0 : (dr > sr ? 1 : -1));
    int cstep = (dc == sc ? 0 : (dc > sc ? 1 : -1));

    int r = sr + rstep, c = sc + cstep;
    while (r != dr || c != dc) {
        if (!isEmpty(board[r][c])) {
            return false;
        }
        r += rstep;
        c += cstep;
    }
    return true;
}

bool ChessGame::isPseudoLegalBasic(int sr, int sc, int dr, int dc) const {

    PieceType src = board[sr][sc];
    PieceType dst = board[dr][dc];

    if (!inBounds(sr, sc) || !inBounds(dr, dc))
        return false;

    if (sr == dr && sc == dc)
        return false;

    if (!isEmpty(dst) && sameColor(src, dst))
        return false;


    if (src == WHITE_PAWN) {
        int dy = dr - sr;
        int dx = dc - sc;

        if (dx == 0 && dy == -1 && isEmpty(dst))
            return true;

        if (dx == 0 && dy == -2 && sr == 6 &&
            isEmpty(board[sr - 1][sc]) && isEmpty(dst))
            return true;

        if ((dx == 1 || dx == -1) && dy == -1 &&
            !isEmpty(dst) && isBlackPiece(dst))
            return true;

        return false;
    }

    if (src == BLACK_PAWN) {
        int dy = dr - sr;
        int dx = dc - sc;

        if (dx == 0 && dy == 1 && isEmpty(dst))
            return true;

        if (dx == 0 && dy == 2 && sr == 1 &&
            isEmpty(board[sr + 1][sc]) && isEmpty(dst))
            return true;

        if ((dx == 1 || dx == -1) && dy == 1 &&
            !isEmpty(dst) && isWhitePiece(dst))
            return true;

        return false;
    }

    if (src == WHITE_KNIGHT || src == BLACK_KNIGHT) {
        int ar = std::abs(dr - sr);
        int ac = std::abs(dc - sc);
        return (ar == 1 && ac == 2) || (ar == 2 && ac == 1);
    }

    if (src == WHITE_BISHOP || src == BLACK_BISHOP) {
        if (std::abs(dr - sr) != std::abs(dc - sc))
            return false;
        return isPathClear(sr, sc, dr, dc);
    }

    if (src == WHITE_ROOK || src == BLACK_ROOK) {
        if (!(sr == dr || sc == dc))
            return false;
        return isPathClear(sr, sc, dr, dc);
    }

    if (src == WHITE_QUEEN || src == BLACK_QUEEN) {
        bool diagonal = (std::abs(dr - sr) == std::abs(dc - sc));
        bool straight  = (sr == dr || sc == dc);
        if (!diagonal && !straight)
            return false;
        return isPathClear(sr, sc, dr, dc);
    }

    if (src == WHITE_KING || src == BLACK_KING) {
        int ar = std::abs(dr - sr);
        int ac = std::abs(dc - sc);
        return (ar <= 1 && ac <= 1 && !(ar == 0 && ac == 0));
    }

    return false;
}

std::pair<int,int> ChessGame::findKing(bool white) const {
    PieceType target = white ? WHITE_KING : BLACK_KING;
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (board[r][c] == target)
                return {r, c};
    return {-1, -1};
}

bool ChessGame::isSquareAttackedBy(int r, int c, bool byWhite) const {
    if (byWhite) {
        if (inBounds(r+1, c-1) && board[r+1][c-1] == WHITE_PAWN) return true;
        if (inBounds(r+1, c+1) && board[r+1][c+1] == WHITE_PAWN) return true;
    } else {
        if (inBounds(r-1, c-1) && board[r-1][c-1] == BLACK_PAWN) return true;
        if (inBounds(r-1, c+1) && board[r-1][c+1] == BLACK_PAWN) return true;
    }

    static const int kOff[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
    for (auto &d : kOff) {
        int rr = r + d[0], cc = c + d[1];
        if (!inBounds(rr, cc)) continue;
        PieceType p = board[rr][cc];
        if (byWhite ? (p == WHITE_KNIGHT) : (p == BLACK_KNIGHT)) return true;
    }

    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int rr = r + dr, cc = c + dc;
            if (!inBounds(rr, cc)) continue;
            PieceType p = board[rr][cc];
            if (byWhite ? (p == WHITE_KING) : (p == BLACK_KING)) return true;
        }
    }

    auto firstPieceHits = [&](int rstep, int cstep) -> PieceType {
        int rr = r + rstep, cc = c + cstep;
        while (inBounds(rr, cc)) {
            if (!isEmpty(board[rr][cc])) return board[rr][cc];
            rr += rstep; cc += cstep;
        }
        return static_cast<PieceType>(-1);
    };

    {
        PieceType p;
        p = firstPieceHits(-1,-1);
        if (byWhite ? (p==WHITE_BISHOP||p==WHITE_QUEEN) : (p==BLACK_BISHOP||p==BLACK_QUEEN)) return true;
        p = firstPieceHits(-1, 1);
        if (byWhite ? (p==WHITE_BISHOP||p==WHITE_QUEEN) : (p==BLACK_BISHOP||p==BLACK_QUEEN)) return true;
        p = firstPieceHits( 1,-1);
        if (byWhite ? (p==WHITE_BISHOP||p==WHITE_QUEEN) : (p==BLACK_BISHOP||p==BLACK_QUEEN)) return true;
        p = firstPieceHits( 1, 1);
        if (byWhite ? (p==WHITE_BISHOP||p==WHITE_QUEEN) : (p==BLACK_BISHOP||p==BLACK_QUEEN)) return true;
    }

    {
        PieceType p;
        p = firstPieceHits(-1, 0);
        if (byWhite ? (p==WHITE_ROOK||p==WHITE_QUEEN) : (p==BLACK_ROOK||p==BLACK_QUEEN)) return true;
        p = firstPieceHits( 1, 0);
        if (byWhite ? (p==WHITE_ROOK||p==WHITE_QUEEN) : (p==BLACK_ROOK||p==BLACK_QUEEN)) return true;
        p = firstPieceHits( 0,-1);
        if (byWhite ? (p==WHITE_ROOK||p==WHITE_QUEEN) : (p==BLACK_ROOK||p==BLACK_QUEEN)) return true;
        p = firstPieceHits( 0, 1);
        if (byWhite ? (p==WHITE_ROOK||p==WHITE_QUEEN) : (p==BLACK_ROOK||p==BLACK_QUEEN)) return true;
    }

    return false;
}


PieceType ChessGame::getPieceAt(int row, int col) {
    return board[row][col];
}

bool ChessGame::hasAnyLegalMoves(bool white) {
    for (int sr = 0; sr < 8; ++sr) {
        for (int sc = 0; sc < 8; ++sc) {
            PieceType src = board[sr][sc];
            if (isEmpty(src)) continue;
            if (white && !isWhitePiece(src)) continue;
            if (!white && !isBlackPiece(src)) continue;

            for (int dr = 0; dr < 8; ++dr) {
                for (int dc = 0; dc < 8; ++dc) {
                    if (!isPseudoLegalBasic(sr, sc, dr, dc)) continue;

                    PieceType savedDst = board[dr][dc];
                    board[dr][dc] = src;
                    board[sr][sc] = static_cast<PieceType>(-1);

                    auto [kr, kc] = findKing(white);
                    bool illegal = isSquareAttackedBy(kr, kc, !white);

                    board[sr][sc] = src;
                    board[dr][dc] = savedDst;

                    if (!illegal)
                        return true;
                }
            }
        }
    }
    return false;
}

bool ChessGame::hasInsufficientMaterial() const {
    int whiteCount = 0, blackCount = 0;
    int whiteMinor = 0, blackMinor = 0;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            PieceType p = board[r][c];
            if (isEmpty(p)) continue;

            if (isWhitePiece(p)) {
                whiteCount++;
                if (p == WHITE_BISHOP || p == WHITE_KNIGHT)
                    whiteMinor++;
                else if (p != WHITE_KING)
                    return false;
            } else {
                blackCount++;
                if (p == BLACK_BISHOP || p == BLACK_KNIGHT)
                    blackMinor++;
                else if (p != BLACK_KING)
                    return false;
            }
        }
    }

    if (whiteCount == 1 && blackCount == 1)
        return true;

    if ((whiteCount == 2 && whiteMinor == 1 && blackCount == 1) ||
        (blackCount == 2 && blackMinor == 1 && whiteCount == 1))
        return true;

    return false;
}

bool ChessGame::makeMove(int srcRow, int srcCol, int destRow, int destCol) {
    if (!inBounds(srcRow, srcCol) || !inBounds(destRow, destCol))
        return false;

    PieceType srcPiece = board[srcRow][srcCol];
    if (isEmpty(srcPiece)) return false;

    bool movingWhite = isWhitePiece(srcPiece);
    if (movingWhite != whiteTurn) return false;

    if ((srcPiece == WHITE_KING || srcPiece == BLACK_KING) && std::abs(destCol - srcCol) == 2) {
        bool isWhite = isWhitePiece(srcPiece);
        int row = srcRow;
        bool kingside = destCol > srcCol;

        if (isWhite) {
            if (whiteKingMoved) return false;
            if (kingside && whiteRookRightMoved) return false;
            if (!kingside && whiteRookLeftMoved) return false;
        } else {
            if (blackKingMoved) return false;
            if (kingside && blackRookRightMoved) return false;
            if (!kingside && blackRookLeftMoved) return false;
        }

        int rookCol = kingside ? 7 : 0;
        int step = kingside ? 1 : -1;
        for (int c = srcCol + step; c != rookCol; c += step) {
            if (!isEmpty(board[row][c])) return false;
        }

        for (int c = srcCol; c != destCol + step; c += step) {
            auto [kr, kc] = findKing(isWhite);
            PieceType old = board[row][c];
            board[row][srcCol] = static_cast<PieceType>(-1);
            board[row][c] = srcPiece;
            bool attacked = isSquareAttackedBy(row, c, !isWhite);
            board[row][c] = old;
            board[row][srcCol] = srcPiece;
            if (attacked) return false;
        }

        board[row][srcCol] = static_cast<PieceType>(-1);
        board[row][destCol] = srcPiece;
        if (kingside) {
            board[row][destCol - 1] = board[row][7];
            board[row][7] = static_cast<PieceType>(-1);
        } else {
            board[row][destCol + 1] = board[row][0];
            board[row][0] = static_cast<PieceType>(-1);
        }

        if (isWhite) {
            whiteKingMoved = true;
            if (kingside) whiteRookRightMoved = true;
            else whiteRookLeftMoved = true;
        } else {
            blackKingMoved = true;
            if (kingside) blackRookRightMoved = true;
            else blackRookLeftMoved = true;
        }

        whiteTurn = !whiteTurn;
    }

    if (!isPseudoLegalBasic(srcRow, srcCol, destRow, destCol))
        return false;

    bool twoSquarePawnMove = false;
    if (srcPiece == WHITE_PAWN && srcRow == 6 && destRow == 4)
        twoSquarePawnMove = true;
    if (srcPiece == BLACK_PAWN && srcRow == 1 && destRow == 3)
        twoSquarePawnMove = true;

    if ((srcPiece == WHITE_PAWN || srcPiece == BLACK_PAWN) &&
        std::abs(destCol - srcCol) == 1 &&
        board[destRow][destCol] == static_cast<PieceType>(-1))
    {
        if (destCol == enPassantCol && destRow == enPassantRow) {
            int captureRow = (isWhitePiece(srcPiece)) ? destRow + 1 : destRow - 1;
            board[captureRow][destCol] = static_cast<PieceType>(-1);
        }
    }

    PieceType savedDst = board[destRow][destCol];
    board[destRow][destCol] = srcPiece;
    board[srcRow][srcCol]   = static_cast<PieceType>(-1);

    if (srcPiece == WHITE_PAWN || srcPiece == BLACK_PAWN || !isEmpty(savedDst))
        halfMoveClock = 0;
    else
        halfMoveClock++;

    auto [kr, kc] = findKing(movingWhite);
    if (isSquareAttackedBy(kr, kc, !movingWhite)) {
        board[srcRow][srcCol]   = srcPiece;
        board[destRow][destCol] = savedDst;
        return false;
    }

    if (srcPiece == WHITE_PAWN && destRow == 0)
        board[destRow][destCol] = WHITE_QUEEN;
    else if (srcPiece == BLACK_PAWN && destRow == 7)
        board[destRow][destCol] = BLACK_QUEEN;

    if (twoSquarePawnMove) {
        enPassantCol = destCol;
        enPassantRow = (isWhitePiece(srcPiece)) ? destRow + 1 : destRow - 1;
    }

    if (srcPiece == WHITE_KING) whiteKingMoved = true;
    if (srcPiece == BLACK_KING) blackKingMoved = true;
    if (srcPiece == WHITE_ROOK && srcRow == 7 && srcCol == 0) whiteRookLeftMoved = true;
    if (srcPiece == WHITE_ROOK && srcRow == 7 && srcCol == 7) whiteRookRightMoved = true;
    if (srcPiece == BLACK_ROOK && srcRow == 0 && srcCol == 0) blackRookLeftMoved = true;
    if (srcPiece == BLACK_ROOK && srcRow == 0 && srcCol == 7) blackRookRightMoved = true;

    whiteTurn = !whiteTurn;

    if (halfMoveClock >= 100)
        status = GameStatus::DRAW;

    bool nowWhite = whiteTurn;
    auto [kRow, kCol] = findKing(nowWhite);
    bool inCheck = isSquareAttackedBy(kRow, kCol, !nowWhite);
    bool hasMoves = hasAnyLegalMoves(nowWhite);

    if (inCheck && !hasMoves)
        status = nowWhite ? GameStatus::BLACK_WIN : GameStatus::WHITE_WIN;
    else if (!inCheck && !hasMoves)
        status = GameStatus::DRAW;
    else
        status = GameStatus::ONGOING;

    enPassantCol = -1;
    enPassantRow = -1;

    if (hasInsufficientMaterial())
        status = GameStatus::DRAW;

    return true;
}

GameStatus ChessGame::getGameStatus() const {
    return status;
}

bool ChessGame::isWhiteTurn() const {
    return whiteTurn;
}

