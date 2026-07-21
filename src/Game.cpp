#include "Game.h"
#include "Piece.h"
#include <iostream>
#include <cctype>

Game::Game() : turn(Color::White) {}

bool Game::isCheck(Color who) const {
    int kr=-1,kc=-1;
    for(int r=0;r<8;r++){
        for(int c=0;c<8;c++){
            auto p=board.at(r,c);
            if(p && p->getColor()==who && p->getType()==PieceType::King){kr=r;kc=c;break;}
        }
        if(kr>=0) break;
    }
    if(kr<0) return false;

    Color enemy=(who==Color::White)?Color::Black:Color::White;
    for(int r=0;r<8;r++)
        for(int c=0;c<8;c++){
            auto p=board.at(r,c);
            if(p && p->getColor()==enemy && p->isValidMove(r,c,kr,kc,board))
                return true;
        }
    return false;
}

bool Game::safeMove(int fr,int fc,int tr,int tc) const {
    Board sim=board;
    auto piece=sim.at(fr,fc);
    if(!piece) return false;

    //
    if(piece->getType()==PieceType::Pawn && fc!=tc && sim.at(tr,tc)==nullptr){
        int capRow=(piece->getColor()==Color::White)?tr-1:tr+1;
        sim.setPiece(capRow,tc,nullptr);
    }

    sim.setPiece(tr,tc,sim.at(fr,fc));
    sim.setPiece(fr,fc,nullptr);

    Color myColor=piece->getColor();
    for(int r=0;r<8;r++)
        for(int c=0;c<8;c++){
            auto kp=sim.at(r,c);
            if(kp && kp->getColor()==myColor && kp->getType()==PieceType::King){
                Color enemy=(myColor==Color::White)?Color::Black:Color::White;
                for(int r2=0;r2<8;r2++)
                    for(int c2=0;c2<8;c2++){
                        auto ep=sim.at(r2,c2);
                        if(ep && ep->getColor()==enemy && ep->isValidMove(r2,c2,r,c,sim))
                            return false;
                    }
                return true;
            }
        }
    return false;
}

Board Game::tryMove(int fr,int fc,int tr,int tc) const {
    Board sim=board;
    sim.setPiece(tr,tc,sim.at(fr,fc));
    sim.setPiece(fr,fc,nullptr);
    return sim;
}

bool Game::hasAnyMove(Color who) const {
    for(int r1=0;r1<8;r1++)
        for(int c1=0;c1<8;c1++){
            auto p=board.at(r1,c1);
            if(!p || p->getColor()!=who) continue;
            for(int r2=0;r2<8;r2++)
                for(int c2=0;c2<8;c2++){
                    if(r1==r2 && c1==c2) continue;
                    if(!p->isValidMove(r1,c1,r2,c2,board)) continue;
                    if(safeMove(r1,c1,r2,c2)) return true;
                }
        }
    return false;
}

void Game::doPromotion(int tr,int tc) {
    auto p=board.at(tr,tc);
    if(!p || p->getType()!=PieceType::Pawn) return;
    int target=(p->getColor()==Color::White)?7:0;
    if(tr!=target) return;

    cout<<"promote to: 1)Q 2)R 3)B 4)N : ";
    int choice;
    cin>>choice;
    while(choice<1 || choice>4){
        cout<<"1-4: ";cin>>choice;
    }
    shared_ptr<Piece> prom;
    switch(choice){
        case 1: prom=make_shared<Queen>(p->getColor());break;
        case 2: prom=make_shared<Rook>(p->getColor());break;
        case 3: prom=make_shared<Bishop>(p->getColor());break;
        case 4: prom=make_shared<Knight>(p->getColor());break;
    }
    board.setPiece(tr,tc,prom);
}

bool Game::tryCastle(int fr,int fc,int tr,int tc) {
    if(abs(tc-fc)!=2 || fr!=tr) return false;
    auto king=board.at(fr,fc);
    if(!king || king->getType()!=PieceType::King || king->hasMoved()) return false;
    if(isCheck(turn)) return false;

    int row=fr;
    if(tc==6){
        if(board.at(row,5) || board.at(row,6)) return false;
        auto rook=board.at(row,7);
        if(!rook || rook->getType()!=PieceType::Rook || rook->hasMoved()) return false;
        board.setPiece(row,5,rook);
        board.setPiece(row,7,nullptr);
        board.setPiece(tr,tc,king);
        board.setPiece(fr,fc,nullptr);
        king->setMoved();
        return true;
    }
    if(tc==2){
        if(board.at(row,1) || board.at(row,2) || board.at(row,3)) return false;
        auto rook=board.at(row,0);
        if(!rook || rook->getType()!=PieceType::Rook || rook->hasMoved()) return false;
        board.setPiece(row,3,rook);
        board.setPiece(row,0,nullptr);
        board.setPiece(tr,tc,king);
        board.setPiece(fr,fc,nullptr);
        king->setMoved();
        return true;
    }
    return false;
}

void Game::start() {
    view.showWelcome();
    board.setupBoard();

    while(true){
        view.showBoard(board);
        Color enemy=(turn==Color::White)?Color::Black:Color::White;

        if(isCheck(turn) && !hasAnyMove(turn)){
            view.showBoard(board);
            view.showWin(enemy==Color::White?"White":"Black");
            break;
        }
        if(!isCheck(turn) && !hasAnyMove(turn)){
            view.showBoard(board);
            view.showDraw();
            break;
        }
        if(isCheck(turn))
            view.showCheck(turn==Color::White?"White":"Black");

        string who=(turn==Color::White)?"White":"Black";
        view.showTurn(who);

        string in,in2;
        if(!(cin>>in)) break;
        if(!(cin>>in2)) in+=in2;

        string fromS,toS;
        if(in2.size()==2 && in.size()==2){fromS=in;toS=in2;}
        else if(in.size()==4){fromS=in.substr(0,2);toS=in.substr(2,2);}
        else{cout<<"bad format\n";continue;}

        int fr=fromS[1]-'1';
        int fc=tolower(fromS[0])-'a';
        int tr=toS[1]-'1';
        int tc=tolower(toS[0])-'a';

        if(fr<0||fr>7||fc<0||fc>7||tr<0||tr>7||tc<0||tc>7){cout<<"bad coords\n";continue;}

        auto piece=board.at(fr,fc);
        if(!piece){cout<<"nothing there\n";continue;}
        if(piece->getColor()!=turn){cout<<"not your piece\n";continue;}
        if(!piece->isValidMove(fr,fc,tr,tc,board)){cout<<"cant move there\n";continue;}

        //castling
        if(tryCastle(fr,fc,tr,tc)){turn=enemy;continue;}

        //en passant
        if(piece->getType()==PieceType::Pawn && fc!=tc && board.at(tr,tc)==nullptr){
            int capRow=(piece->getColor()==Color::White)?tr-1:tr+1;
            board.setPiece(capRow,tc,nullptr);
        }

        if(!safeMove(fr,fc,tr,tc)){cout<<"that leaves you in check\n";continue;}

        board.setPiece(tr,tc,board.at(fr,fc));
        board.setPiece(fr,fc,nullptr);
        if(board.at(tr,tc)) board.at(tr,tc)->setMoved();

        doPromotion(tr,tc);
        turn=enemy;
    }
    cout<<"\ngame over\n";
}
