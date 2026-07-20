#include <iostream>
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
    b.print();

    return 0;
}
