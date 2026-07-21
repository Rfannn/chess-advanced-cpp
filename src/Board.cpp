#include "Board.h"
#include <iostream>

Board::Board() {
    grid.resize(8, vector<shared_ptr<Piece>>(8, nullptr));
    setupBoard();
}

void Board::setupBoard() {
    //todo:fill w pieces
    grid[7][0]=make_shared<Rook>(Color::Black);
    grid[7][1]=make_shared<Knight>(Color::Black);
    grid[7][2]=make_shared<Bishop>(Color::Black);
    grid[7][3]=make_shared<Queen>(Color::Black);
    grid[7][4]=make_shared<King>(Color::Black);
    grid[7][5]=make_shared<Bishop>(Color::Black);
    grid[7][6]=make_shared<Knight>(Color::Black);
    grid[7][7]=make_shared<Rook>(Color::Black);
    for(int c=0;c<8;c++) grid[6][c]=make_shared<Pawn>(Color::Black);

    grid[0][0]=make_shared<Rook>(Color::White);
    grid[0][1]=make_shared<Knight>(Color::White);
    grid[0][2]=make_shared<Bishop>(Color::White);
    grid[0][3]=make_shared<Queen>(Color::White);
    grid[0][4]=make_shared<King>(Color::White);
    grid[0][5]=make_shared<Bishop>(Color::White);
    grid[0][6]=make_shared<Knight>(Color::White);
    grid[0][7]=make_shared<Rook>(Color::White);
    for(int c=0;c<8;c++) grid[1][c]=make_shared<Pawn>(Color::White);
}

shared_ptr<Piece> Board::at(int row, int col) const {
    if(row<0 || row>7 || col<0 || col>7) return nullptr;
    return grid[row][col];
}

void Board::setPiece(int row, int col, shared_ptr<Piece> p) {
    if(row>=0 && row<=7 && col>=0 && col<=7)
        grid[row][col]=p;
}

void Board::print() const {
    for(int r=7;r>=0;r--){
        cout<<(r+1)<<" ";
        for(int c=0;c<8;c++){
            auto p=grid[r][c];
            if(!p) cout<<". ";
            else cout<<p->getSymbol()<<" ";
        }
        cout<<endl;
    }
    cout<<"  a b c d e f g h"<<endl;
}
