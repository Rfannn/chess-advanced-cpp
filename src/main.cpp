#include <iostream>
#include <memory>
#include "common.h"
#include "Board.h"

int main() {
    Position p=Position::fromString("e4");
    if (p.isValid()) {
        cout << "e4 -> row="<< p.row <<" col="<<p.col<<endl;
    } else {
        cout << "invalid position\n";
    }

    Board b;
    // manually place a couple pieces to test
    b.setPiece(0, 4, make_shared<King>(Color::White));
    b.setPiece(7, 4, make_shared<King>(Color::Black));
    b.setPiece(0, 3, make_shared<Queen>(Color::White));
    b.print();

    return 0;
}
