#include "Piece.h"
#include "Board.h"
#include <cmath>

bool Pawn::isValidMove(int fr, int fc, int tr, int tc, const Board& b) const {
    int dir =(color==Color::White)? 1 : -1;
    int startRow= (color==Color::White) ? 1 : 6;
    int dr=tr-fr;
    int dc= tc-fc;

    //diag
    if (tc != fc && dr == dir && abs(dc) == 1) {
        auto target = b.at(tr, tc);
        if (target && target->getColor() != color) return true;
        return false;
    }

    //forward
    if (tc == fc && dr == dir && b.at(tr, tc) == nullptr) return true;

    //double
    if (fr==startRow && tc==fc && dr==2*dir && b.at(tr, tc) ==nullptr && b.at(fr + dir, fc)==nullptr)
        return true;

    return false;
}

bool Knight::isValidMove(int fr,int fc,int tr, int tc, const Board&) const {
    int dr = abs(tr - fr);
    int dc = abs(tc- fc);
    return ((dr == 2 && dc == 1) || (dr == 1 && dc == 2));
}

bool Bishop::isValidMove(int fr,int fc,int tr,int tc, const Board& b) const {
    int dr=tr-fr;
    int dc=tc-fc;
    if (abs(dr)!=abs(dc) || dr==0) return false;
    int stepR=(dr>0)?1:-1;
    int stepC=(dc>0)?1:-1;
    for(int r=fr+stepR,c=fc+stepC;r!=tr;r+=stepR,c+=stepC)
        if(b.at(r,c)) return false;
    return true;
}

bool Rook::isValidMove(int fr,int fc,int tr,int tc, const Board& b) const {
    int dr=tr-fr;
    int dc=tc-fc;
    if (dr!=0 && dc!=0) return false;
    if (dr!=0) {
        int step=(dr>0)?1:-1;
        for(int r=fr+step;r!=tr;r+=step)
            if(b.at(r,fc)) return false;
    }
    if (dc!=0) {
        int step=(dc>0)?1:-1;
        for(int c=fc+step;c!=tc;c+=step)
            if(b.at(fr,c)) return false;
    }
    return true;
}

bool Queen::isValidMove(int fr,int fc,int tr,int tc, const Board& b) const {
    return Rook(color).isValidMove(fr,fc,tr,tc,b)
        || Bishop(color).isValidMove(fr,fc,tr,tc,b);
}

bool King::isValidMove(int fr,int fc,int tr,int tc, const Board&) const {
    int dr=abs(tr-fr);
    int dc=abs(tc-fc);
    if (dr<=1 && dc<=1 && !(dr==0 && dc==0)) return true;
    return false;
}
