#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Renderer.h"
#include <string>
#include <fstream>

using namespace std;

class Game {
private:
    Board board;
    Renderer view;
    Color turn;

    bool isCheck(Color who) const;
    bool hasAnyMove(Color who) const;
    bool safeMove(int fr, int fc, int tr, int tc) const;
    Board tryMove(int fr, int fc, int tr, int tc) const;
    void doPromotion(int tr, int tc);
    bool tryCastle(int fr, int fc, int tr, int tc);
    
    //bonus
    void saveGame();
    void loadGame();

public:
    Game();
    void start();
};

#endif
