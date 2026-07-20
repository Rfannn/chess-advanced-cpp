#include <iostream>
#include "common.h"

int main() {
    Position p = Position::fromString("e4");
    if (p.isValid()) {
        cout << "e4 -> row=" << p.row << " col=" << p.col << endl;
        cout << "back to string: " << p.toString() << endl;
    } else {
        cout << "invalid position\n";
    }

    Position p2 = Position::fromString("a1");
    cout << "a1 -> row=" << p2.row << " col=" << p2.col << endl;
    cout << "back: " << p2.toString() << endl;

    return 0;
}
