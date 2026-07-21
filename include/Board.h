#pragma once

#include "common.h"
#include "Piece.h"
#include <memory>
#include <vector>

using namespace std;

class Board {
private:
    vector<vector<shared_ptr<Piece>>> grid;

public:
    Board();

    shared_ptr<Piece> at(int row, int col) const;
    void setPiece(int row, int col, shared_ptr<Piece> p);
    void setupBoard();
    void print() const;
};
