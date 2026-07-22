#include "Game.h"
#include "Piece.h"
#include <iostream>
#include <cctype>

Game::Game() : turn(Color::White), enPassantTarget(-1, -1) {}

void Game::clearEnPassantTarget() {
    enPassantTarget = Position(-1, -1);
}

bool Game::squareAttacked(int r, int c, Color by) const {
    if (r < 0 || r > 7 || c < 0 || c > 7) return false;
    Color enemy = by;
    for (int rr = 0; rr < 8; ++rr) {
        for (int cc = 0; cc < 8; ++cc) {
            auto p = board.at(rr, cc);
            if (p && p->getColor() == enemy) {
                if (p->isValidMove(rr, cc, r, c, board)) return true;
            }
        }
    }
    return false;
}

bool Game::isCheck(Color who) const {
    int kr = -1, kc = -1;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            auto p = board.at(r, c);
            if (p && p->getColor() == who && p->getType() == PieceType::King) {
                kr = r; kc = c;
                break;
            }
        }
        if (kr >= 0) break;
    }
    if (kr < 0) return false;

    Color enemy = (who == Color::White) ? Color::Black : Color::White;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            auto p = board.at(r, c);
            if (p && p->getColor() == enemy) {
                if (p->isValidMove(r, c, kr, kc, board)) return true;
            }
        }
    }
    return false;
}

bool Game::safeMove(int fr, int fc, int tr, int tc) const {
    Board sim = board;
    auto piece = sim.at(fr, fc);
    if (!piece) return false;

    //en passant capture
    if (piece->getType() == PieceType::Pawn && fc != tc && sim.at(tr, tc) == nullptr) {
        int capRow = (piece->getColor() == Color::White) ? tr - 1 : tr + 1;
        sim.setPiece(capRow, tc, nullptr);
    }

    sim.setPiece(tr, tc, sim.at(fr, fc));
    sim.setPiece(fr, fc, nullptr);

    Color myColor = piece->getColor();
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            auto kp = sim.at(r, c);
            if (kp && kp->getColor() == myColor && kp->getType() == PieceType::King) {
                Color enemy = (myColor == Color::White) ? Color::Black : Color::White;
                for (int r2 = 0; r2 < 8; ++r2) {
                    for (int c2 = 0; c2 < 8; ++c2) {
                        auto ep = sim.at(r2, c2);
                        if (ep && ep->getColor() == enemy && ep->isValidMove(r2, c2, r, c, sim))
                            return false;
                    }
                }
                return true;
            }
        }
    }
    return false;
}

bool Game::hasAnyMove(Color who) const {
    for (int r1 = 0; r1 < 8; ++r1) {
        for (int c1 = 0; c1 < 8; ++c1) {
            auto p = board.at(r1, c1);
            if (!p || p->getColor() != who) continue;
            for (int r2 = 0; r2 < 8; ++r2) {
                for (int c2 = 0; c2 < 8; ++c2) {
                    if (r1 == r2 && c1 == c2) continue;
                    if (!p->isValidMove(r1, c1, r2, c2, board)) continue;
                    if (safeMove(r1, c1, r2, c2)) return true;
                }
            }
        }
    }
    return false;
}

void Game::doPromotion(int tr, int tc) {
    auto p = board.at(tr, tc);
    if (!p || p->getType() != PieceType::Pawn) return;
    int target = (p->getColor() == Color::White) ? 7 : 0;
    if (tr != target) return;

    cout << "promote to: 1)Q 2)R 3)B 4)N : ";
    int choice;
    if (!(cin >> choice)) { cin.clear(); cin.ignore(1000, '\n'); return; }

    shared_ptr<Piece> prom;
    if (choice == 1) prom = make_shared<Queen>(p->getColor());
    else if (choice == 2) prom = make_shared<Rook>(p->getColor());
    else if (choice == 3) prom = make_shared<Bishop>(p->getColor());
    else prom = make_shared<Knight>(p->getColor());

    board.setPiece(tr, tc, prom);
}

bool Game::tryCastle(int fr, int fc, int tr, int tc) {
    if (abs(tc - fc) != 2 || fr != tr) return false;
    auto king = board.at(fr, fc);
    if (!king || king->getType() != PieceType::King || king->getHasMoved()) return false;
    if (isCheck(turn)) return false;

    int row = fr;
    Color enemy = (turn == Color::White) ? Color::Black : Color::White;

    if (tc == 6) { //kingside
        if (board.at(row, 5) || board.at(row, 6)) return false;
        //check the squares that the king goes through (f and g files)
        if (squareAttacked(row, 5, enemy) || squareAttacked(row, 6, enemy)) return false;
        auto rook = board.at(row, 7);
        if (!rook || rook->getType() != PieceType::Rook || rook->getHasMoved()) return false;
        board.setPiece(row, 5, rook);
        board.setPiece(row, 7, nullptr);
        board.setPiece(tr, tc, king);
        board.setPiece(fr, fc, nullptr);
        king->setHasMoved();
        rook->setHasMoved();
        return true;
    }
    if (tc == 2) { // queenside
        if (board.at(row, 1) || board.at(row, 2) || board.at(row, 3)) return false;
        //  check the squares that the king goes through(d and c files)
        if (squareAttacked(row, 3, enemy) || squareAttacked(row, 2, enemy)) return false;
        auto rook = board.at(row, 0);
        if (!rook || rook->getType() != PieceType::Rook || rook->getHasMoved()) return false;
        board.setPiece(row, 3, rook);
        board.setPiece(row, 0, nullptr);
        board.setPiece(tr, tc, king);
        board.setPiece(fr, fc, nullptr);
        king->setHasMoved();
        rook->setHasMoved();
        return true;
    }
    return false;
}

void Game::saveGame() {
    ofstream out("chess_save.txt");
    if (!out) { cout << "save failed\n"; return; }
    out << (turn == Color::White ? 0 : 1) << endl;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            auto p = board.at(r, c);
            if (!p) out << "-1 -1 0 ";
            else out << (int)p->getType() << " " << (int)p->getColor() << " " << (p->getHasMoved() ? 1 : 0) << " ";
        }
        out << endl;
    }
    //save en passant target
    out << enPassantTarget.row << " " << enPassantTarget.col << endl;
    cout << "saved.\n";
}

void Game::loadGame() {
    ifstream in("chess_save.txt");
    if (!in) { cout << "no save file\n"; return; }
    int t; in >> t;
    turn = (t == 0) ? Color::White : Color::Black;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            int type, col, moved;
            in >> type >> col >> moved;
            if (type == -1) board.setPiece(r, c, nullptr);
            else {
                Color pieceCol = (col == 0) ? Color::White : Color::Black;
                shared_ptr<Piece> p;
                if (type == 0) p = make_shared<Pawn>(pieceCol);
                else if (type == 1) p = make_shared<Knight>(pieceCol);
                else if (type == 2) p = make_shared<Bishop>(pieceCol);
                else if (type == 3) p = make_shared<Rook>(pieceCol);
                else if (type == 4) p = make_shared<Queen>(pieceCol);
                else p = make_shared<King>(pieceCol);
                if (moved) p->setHasMoved();
                board.setPiece(r, c, p);
            }
        }
    }
    int epRow, epCol;
    in >> epRow >> epCol;
    enPassantTarget = Position(epRow, epCol);
    cout << "loaded.\n";
}

void Game::start() {
    view.showWelcome();
    board.setupBoard();

    while (true) {
        view.showBoard(board);
        Color enemy = (turn == Color::White) ? Color::Black : Color::White;

        if (isCheck(turn) && !hasAnyMove(turn)) {
            view.showBoard(board);
            view.showWin(enemy == Color::White ? "White" : "Black");
            break;
        }
        if (!isCheck(turn) && !hasAnyMove(turn)) {
            view.showBoard(board);
            view.showDraw();
            break;
        }
        if (isCheck(turn)) view.showCheck(turn == Color::White ? "White" : "Black");

        string who = (turn == Color::White) ? "White" : "Black";
        cout << who << "'s turn. move (or 'save'/'load'): ";

        string in;
        if (!(cin >> in)) break;
        if (in == "save") { saveGame(); continue; }
        if (in == "load") { loadGame(); continue; }
        if (in == "exit" || in == "quit") break;

        string in2;
        if (!(cin >> in2)) break;

        string fromS, toS;
        if (in2.size() == 2 && in.size() == 2) { fromS = in; toS = in2; }
        else if (in.size() == 4) { fromS = in.substr(0, 2); toS = in.substr(2, 2); }
        else { cout << "bad format\n"; continue; }

        int fr = fromS[1] - '1';
        int fc = tolower(fromS[0]) - 'a';
        int tr = toS[1] - '1';
        int tc = tolower(toS[0]) - 'a';

        if (fr < 0 || fr > 7 || fc < 0 || fc > 7 || tr < 0 || tr > 7 || tc < 0 || tc > 7) { cout << "bad coords\n"; continue; }

        auto piece = board.at(fr, fc);
        if (!piece) { cout << "nothing there\n"; continue; }
        if (piece->getColor() != turn) { cout << "not your piece\n"; continue; }

        //stop the king from moving into check
        if (piece->getType() == PieceType::King && abs(tc - fc) == 2 && fr == tr) {
            if (tryCastle(fr, fc, tr, tc)) {
                clearEnPassantTarget();
                turn = enemy;
                continue;
            } else {
                cout << "cant castle\n";
                continue;
            }
        }

        if (!piece->isValidMove(fr, fc, tr, tc, board)) { cout << "cant move there\n"; continue; }

        //check friendlyfire
        auto target = board.at(tr, tc);
        if (target && target->getColor() == piece->getColor()) {
            cout << "cant capture own piece\n";
            continue;
        }

        //en passant check
        bool isEnPassant = false;
        if (piece->getType() == PieceType::Pawn && fc != tc && board.at(tr, tc) == nullptr) {
            if (enPassantTarget.row == tr && enPassantTarget.col == tc) {
                isEnPassant = true;
            } else {
                cout << "cant move there\n";
                continue;
            }
        }

        if (!safeMove(fr, fc, tr, tc)) { cout << "that leaves you in check\n"; continue; }

        //do the move
        if (isEnPassant) {
            int capRow = (piece->getColor() == Color::White) ? tr - 1 : tr + 1;
            board.setPiece(capRow, tc, nullptr);
        }

        board.setPiece(tr, tc, board.at(fr, fc));
        board.setPiece(fr, fc, nullptr);
        if (board.at(tr, tc)) board.at(tr, tc)->setHasMoved();

        //update en passant target
        clearEnPassantTarget();
        if (piece->getType() == PieceType::Pawn && abs(tr - fr) == 2) {
            int targetRow = (piece->getColor() == Color::White) ? tr - 1 : tr + 1;
            enPassantTarget = Position(targetRow, tc);
        }

        doPromotion(tr, tc);
        turn = enemy;
    }
}