#ifndef COMMON_H
#define COMMON_H

#include <string>
using namespace std;

enum class Color {White,Black };
enum class PieceType {Pawn,Knight, Bishop,Rook,Queen,King};

struct Position {
    int row;
    int col;

    bool isValid() const {
        return row>=0 && row<8 && col>=0 && col<8;
    }

    string toString() const {
        string s;
        s += static_cast<char>('a' + col);
        s += static_cast<char>('1' + row);
        return s;
    }

    static Position fromString(const string& s);
};

#endif
