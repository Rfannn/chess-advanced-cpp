#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Renderer.h"
#include "common.h"
#include <string>
#include <fstream>

using namespace std;

//result of a move attempt
enum class MoveResult { Ok, Illegal, Check, Checkmate, Stalemate, CastleOk, Promotion };

class Game {
private:
    Board board;
    Renderer view;
    Color turn;
    Position enPassantTarget;

    bool isCheck(Color who) const;
    bool hasAnyMove(Color who) const;
    bool safeMove(int fr, int fc, int tr, int tc) const;
    Board tryMove(int fr, int fc, int tr, int tc) const;
    void doPromotion(int tr, int tc);
    bool tryCastle(int fr, int fc, int tr, int tc);
    bool squareAttacked(int r, int c, Color by) const;
    void clearEnPassantTarget();
    
    void saveGame();
    void loadGame();

public:
    Game();
    void start();
    
    //for GUI
    const Board& getBoard() const { return board; }
    Color getTurn() const { return turn; }
    MoveResult makeMove(int fr, int fc, int tr, int tc);
    void doPromotionChoice(int tr, int tc, int choice);
    bool inCheck() const { return isCheck(turn); }
    bool isGameOver() const;
    string getStatusMsg() const;
};

#endif
