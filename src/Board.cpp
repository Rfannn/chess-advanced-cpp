#include "Board.h"
#include <iostream>

Board::Board() {
    grid.resize(8, vector<shared_ptr<Piece>>(8, nullptr));
    setupBoard();
}

void Board::setupBoard() {
    //todo:fill w pieces
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            grid[r][c] = nullptr;
}

shared_ptr<Piece> Board::at(int row, int col) const {
    if (row < 0 || row > 7 || col < 0 || col > 7)
        return nullptr;
    return grid[row][col];
}

void Board::setPiece(int row, int col, shared_ptr<Piece> p) {
    if (row >= 0 && row <= 7 && col >= 0 && col <= 7)
        grid[row][col] = p;
}

void Board::print() const {
    for (int r = 7; r >= 0; --r) {
        cout << (r+1) << " ";
        for (int c = 0; c < 8; ++c) {
            auto p = grid[r][c];
            if (!p) cout << ". ";
            else cout << p->getSymbol() << " ";
        }
        cout << endl;
    }
    cout << "  a b c d e f g h" << endl;
}
