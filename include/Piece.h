#ifndef PIECE_H
#define PIECE_H

#include "common.h"
#include <string>

class Board;

class Piece {
protected:
    Color color;
    PieceType type;
    bool moved;

public:
    Piece(Color c, PieceType t) : color(c), type(t), moved(false) {}
    virtual ~Piece() = default;

    Color getColor() const { return color; }
    PieceType getType() const { return type; }
    bool hasMoved() const { return moved; }
    void markMoved() { moved = true; } // renamed from setMoved

    virtual char getSymbol() const = 0;
    virtual bool isValidMove(int fr, int fc, int tr, int tc, const Board& b) const = 0;
};

class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c, PieceType::Pawn) {}
    char getSymbol() const override { return color == Color::White ? 'P' : 'p'; }
    bool isValidMove(int fr, int fc, int tr, int tc, const Board& b) const override;
};

class Knight : public Piece {
public:
    Knight(Color c) : Piece(c, PieceType::Knight) {}
    char getSymbol() const override { return color == Color::White ? 'N' : 'n'; }
    bool isValidMove(int fr, int fc, int tr, int tc, const Board& b) const override;
};

class Bishop : public Piece {
public:
    Bishop(Color c) : Piece(c, PieceType::Bishop) {}
    char getSymbol() const override { return color == Color::White ? 'B' : 'b'; }
    bool isValidMove(int fr, int fc, int tr, int tc, const Board& b) const override;
};

class Rook : public Piece {
public:
    Rook(Color c) : Piece(c, PieceType::Rook) {}
    char getSymbol() const override { return color == Color::White ? 'R' : 'r'; }
    bool isValidMove(int fr, int fc, int tr, int tc, const Board& b) const override;
};

class Queen : public Piece {
public:
    Queen(Color c) : Piece(c, PieceType::Queen) {}
    char getSymbol() const override { return color == Color::White ? 'Q' : 'q'; }
    bool isValidMove(int fr, int fc, int tr, int tc, const Board& b) const override;
};

class King : public Piece {
public:
    King(Color c) : Piece(c, PieceType::King) {}
    char getSymbol() const override { return color == Color::White ? 'K' : 'k'; }
    bool isValidMove(int fr, int fc, int tr, int tc, const Board& b) const override;
};

#endif
