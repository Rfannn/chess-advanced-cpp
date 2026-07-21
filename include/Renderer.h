#ifndef RENDERER_H
#define RENDERER_H

#include "Board.h"
#include <string>

class Renderer {
public:
    void showBoard(const Board& b) const;
    void showWelcome() const;
    void showTurn(const string& who) const;
    void showCheck(const string& who) const;
    void showWin(const string& who) const;
    void showDraw() const;
};

#endif
