#include "Renderer.h"
#include <iostream>

void Renderer::showWelcome() const {
    cout << "==============================\n";
    cout << "            CHESS        \n";
    cout << "==============================\n\n";
}

void Renderer::showBoard(const Board& b) const {
    cout<<"\n   a b c d e f g h\n";
    cout<<"  +-----------------+\n";
    for(int r=7;r>=0;--r){
        cout <<(r+1)<<"|";
        for (int c = 0; c < 8; ++c) {
            auto p = b.at(r, c);
            if (!p) {
                if ((r+c)%2==0) cout << " . ";
                else cout << "   ";
            } else {
                cout<<" " << p->getSymbol() << " ";
            }
        }
        cout<<"|" << (r+1) << "\n";
    }
    cout<< "  +-----------------+\n";
    cout<< "   a b c d e f g h\n\n";
}

void Renderer::showTurn(const string& who) const {
    cout << who << "'s turn. enter move (ex: e2 e4): ";
}

void Renderer::showCheck(const string& who) const {
    cout << "!!! " << who << " is in check!\n";
}

void Renderer::showWin(const string& who) const {
    cout << "\n"<<who<< " wins!\n";
}

void Renderer::showDraw() const {
    cout << "\nstalemate! draw.\n";
}
