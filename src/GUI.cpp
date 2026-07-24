#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef min
#undef max
#include <windowsx.h>
#include "GUI.h"
#include "Piece.h"
#include <string>

GUIData GUI::data;
HWND GUI::hwnd = nullptr;
HWND GUI::promoDlg = nullptr;
HFONT GUI::pieceFont = nullptr;
HFONT GUI::smallFont = nullptr;
HFONT GUI::coordFont = nullptr;

//piece type to unicode
static const wchar_t* getSym(PieceType t, Color c) {
    if (c == Color::White) {
        switch(t) {
            case PieceType::King: return L"\u2654";
            case PieceType::Queen: return L"\u2655";
            case PieceType::Rook: return L"\u2656";
            case PieceType::Bishop: return L"\u2657";
            case PieceType::Knight: return L"\u2658";
            case PieceType::Pawn: return L"\u2659";
        }
    } else {
        switch(t) {
            case PieceType::King: return L"\u265A";
            case PieceType::Queen: return L"\u265B";
            case PieceType::Rook: return L"\u265C";
            case PieceType::Bishop: return L"\u265D";
            case PieceType::Knight: return L"\u265E";
            case PieceType::Pawn: return L"\u265F";
        }
    }
    return L"";
}

//promote
LRESULT CALLBACK GUI::PromoProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE: {
            //4buttons
            CreateWindowExA(0, "BUTTON", "Queen (\u2655)", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                10, 10, 110, 40, hDlg, (HMENU)1, nullptr, nullptr);
            CreateWindowExA(0, "BUTTON", "Rook (\u2656)", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                130, 10, 110, 40, hDlg, (HMENU)2, nullptr, nullptr);
            CreateWindowExA(0, "BUTTON", "Bishop (\u2657)", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                10, 60, 110, 40, hDlg, (HMENU)3, nullptr, nullptr);
            CreateWindowExA(0, "BUTTON", "Knight (\u2658)", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                130, 60, 110, 40, hDlg, (HMENU)4, nullptr, nullptr);
            return 0;
        }
        case WM_COMMAND: {
            int choice = LOWORD(wParam);
            if(choice >= 1 && choice <= 4) {
                data.game->doPromotionChoice(data.promoTr, data.promoTc, choice);
                data.waitingPromo = false;
                data.status = data.game->getStatusMsg();
                DestroyWindow(hDlg);
                promoDlg = nullptr;
                InvalidateRect(hwnd,nullptr, TRUE);
            }
            return 0;
        }
        case WM_CLOSE:
            //default queen
            data.game->doPromotionChoice(data.promoTr, data.promoTc, 1);
            data.waitingPromo = false;
            data.status = data.game->getStatusMsg();
            DestroyWindow(hDlg);
            promoDlg = nullptr;
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
    }
    return DefWindowProc(hDlg, msg, wParam, lParam);
}

LRESULT CALLBACK GUI::WndProc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE:
            pieceFont = CreateFontW(40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI Symbol");
            smallFont = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Consolas");
            coordFont = CreateFontA(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(h, &ps);
            drawBoard(hdc);
            EndPaint(h, &ps);
            return 0;
        }

        case WM_LBUTTONDOWN: {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            handleClick(x,y);
            return 0;
        }

        case WM_CLOSE:
            DestroyWindow(h);
            return 0;
        case WM_DESTROY:
            if(pieceFont) DeleteObject(pieceFont);
            if(smallFont) DeleteObject(smallFont);
            if(coordFont) DeleteObject(coordFont);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(h,msg, wParam, lParam);
}

void GUI::drawSquareBorder(HDC hdc, int x, int y) {
    //border for square
    HPEN pen = CreatePen(PS_SOLID, 1, RGB(80,80,80));
    HGDIOBJ old = SelectObject(hdc, pen);
    MoveToEx(hdc, x, y, nullptr);
    LineTo(hdc, x+CELL, y);
    LineTo(hdc, x+CELL, y+CELL);
    LineTo(hdc, x, y+CELL);
    LineTo(hdc, x, y);
    SelectObject(hdc, old);
    DeleteObject(pen);
}

void GUI::drawBoard(HDC hdc) {
    Game* game = data.game;
    if(!game) return;

    //board colo
    HBRUSH lightSq = CreateSolidBrush(RGB(238,216,174));
    HBRUSH darkSq = CreateSolidBrush(RGB(162,113,73));
    HBRUSH selSq = CreateSolidBrush(RGB(124,191,54));
    HBRUSH bg = CreateSolidBrush(RGB(30,30,30));
    HBRUSH coordBg = CreateSolidBrush(RGB(50,50,50));

    RECT winRect;
    GetClientRect(hwnd, &winRect);
    FillRect(hdc, &winRect, bg);

    //boards frame
    HPEN framePen = CreatePen(PS_SOLID, 3, RGB(200,200,200));
    HGDIOBJ oldPen = SelectObject(hdc, framePen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, OFFSET_X-3, OFFSET_Y-3, OFFSET_X + 8*CELL + 3, OFFSET_Y + 8*CELL + 3);
    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(framePen);

    const Board& b = game->getBoard();

    //highlight
    bool showHints = (data.selRow != -1);

    for(int r=0;r<8;r++) {
        for(int c=0;c<8;c++) {
            int screenRow = 7 - r;
            int x = OFFSET_X + c * CELL;
            int y = OFFSET_Y + screenRow * CELL;
            RECT sq = {x, y, x+CELL, y+CELL};

            bool isSel = (data.selRow==r && data.selCol==c);
            if(isSel) {
                FillRect(hdc,&sq,selSq);
            } else if((r+c)%2==0) {
                FillRect(hdc,&sq,lightSq);
            } else {
                FillRect(hdc,&sq,darkSq);
            }

            //highlight
            if(showHints && data.selRow != -1) {
                auto selPiece = b.at(data.selRow, data.selCol);
                if(selPiece) {
                    bool capture = (b.at(r,c) != nullptr);
                    if(selPiece->isValidMove(data.selRow, data.selCol, r, c, b) && !isSel) {
                        if(capture) {
                            //highlight capture
                            HBRUSH dot = CreateSolidBrush(RGB(200,50,50));
                            SelectObject(hdc, dot);
                            Ellipse(hdc, x+CELL/2-12, y+CELL/2-12, x+CELL/2+12, y+CELL/2+12);
                            DeleteObject(dot);
                        } else {
                            //highlight move
                            HBRUSH dot = CreateSolidBrush(RGB(50,150,50));
                            SelectObject(hdc, dot);
                            Ellipse(hdc, x+CELL/2-8, y+CELL/2-8, x+CELL/2+8, y+CELL/2+8);
                            DeleteObject(dot);
                        }
                    }
                }
            }

            //border
            drawSquareBorder(hdc, x, y);

            //piece
            auto p = b.at(r,c);
            if(p) {
                const wchar_t* sym = getSym(p->getType(), p->getColor());
                SetBkMode(hdc, TRANSPARENT);
                HFONT oldFont = (HFONT)SelectObject(hdc, pieceFont);
                RECT r2 = {x, y+2, x+CELL, y+CELL+2};
                if(p->getColor() == Color::White) {
                    //white piece with black outline
                    SetTextColor(hdc, RGB(0,0,0));
                    RECT r3 = {x-1, y+1, x+CELL-1, y+CELL+1};
                    DrawTextW(hdc, sym, -1, &r3, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
                    RECT r4 = {x+1, y+1, x+CELL+1, y+CELL+1};
                    DrawTextW(hdc, sym, -1, &r4, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
                    SetTextColor(hdc, RGB(255,255,255));
                    DrawTextW(hdc, sym, -1, &r2, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
                } else {
                    SetTextColor(hdc, RGB(20,20,20));
                    DrawTextW(hdc, sym, -1, &r2, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
                }
                SelectObject(hdc, oldFont);
            }
        }
    }

    //xy
    SelectObject(hdc, coordFont);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(180,180,180));
    for(int c=0;c<8;c++) {
        char label[2] = {(char)('a'+c), 0};
        RECT r = {OFFSET_X + c*CELL, OFFSET_Y + 8*CELL + 5, OFFSET_X + (c+1)*CELL, OFFSET_Y + 8*CELL + 22};
        DrawTextA(hdc, label, 1, &r, DT_CENTER);
    }
    for(int r=0;r<8;r++) {
        char label[2] = {(char)('8'-r), 0};
        RECT rect = {OFFSET_X-25, OFFSET_Y + r*CELL, OFFSET_X-2, OFFSET_Y + (r+1)*CELL};
        DrawTextA(hdc, label, 1, &rect, DT_CENTER|DT_VCENTER);
    }

    //status
    SelectObject(hdc, smallFont);
    SetTextColor(hdc, RGB(255,255,255));
    RECT statusRect = {OFFSET_X, OFFSET_Y + 8*CELL + 30, OFFSET_X + 8*CELL, OFFSET_Y + 8*CELL + 60};
    DrawTextA(hdc, data.status.c_str(), -1, &statusRect, DT_CENTER);

    DeleteObject(lightSq);
    DeleteObject(darkSq);
    DeleteObject(selSq);
    DeleteObject(bg);
    DeleteObject(coordBg);
}

void GUI::handleClick(int x, int y) {
    if(data.waitingPromo) return;

    int screenCol = (x - OFFSET_X) / CELL;
    int screenRow = (y - OFFSET_Y) / CELL;
    if(screenCol < 0 || screenCol > 7 || screenRow < 0 || screenRow > 7) return;

    int col = screenCol;
    int row = 7 - screenRow;

    Game* game = data.game;

    if(data.selRow == -1) {
        auto p = game->getBoard().at(row, col);
        if(p && p->getColor() == game->getTurn()) {
            data.selRow = row;
            data.selCol = col;
            data.status = "selected " + string(1, p->getSymbol());
        }
    } else {
        if(data.selRow == row && data.selCol == col) {
            //deselect
            data.selRow = -1;
            data.selCol = -1;
            data.status = game->getStatusMsg();
        } else {
            doMove(data.selRow, data.selCol, row, col);
            data.selRow = -1;
            data.selCol = -1;
        }
    }

    InvalidateRect(hwnd, nullptr, TRUE);
}

void GUI::doMove(int fr, int fc, int tr, int tc) {
    Game* game = data.game;
    MoveResult res = game->makeMove(fr, fc, tr, tc);

    if(res == MoveResult::Illegal) {
        data.status = "illegal move!";
    } else if(res == MoveResult::Promotion) {
        data.promoTr = tr;
        data.promoTc = tc;
        data.waitingPromo = true;
        showPromotionDialog();
    } else if(res == MoveResult::Checkmate) {
        string winner = (game->getTurn() == Color::White) ? "Black" : "White";
        data.status = "Checkmate! " + winner + " wins!";
    } else if(res == MoveResult::Stalemate) {
        data.status = "Stalemate! Draw.";
    } else if(res == MoveResult::Check) {
        data.status = "Check!";
    } else {
        data.status = game->getStatusMsg();
    }
}

void GUI::showPromotionDialog() {
    //custom popup
    const char* promoClass = "ChessPromo";
    WNDCLASSA wc = {};
    wc.lpfnWndProc = PromoProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = promoClass;
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    RegisterClassA(&wc);

    promoDlg = CreateWindowExA(WS_EX_TOOLWINDOW, promoClass, "Choose Promotion Piece",
        WS_POPUP|WS_CAPTION|WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 260, 150,
        hwnd, nullptr, GetModuleHandle(nullptr), nullptr);
}

void GUI::run(Game& g) {
    data.game = &g;
    data.selRow = -1;
    data.selCol = -1;
    data.status = "White's turn - click a piece";
    data.waitingPromo = false;
    data.promoTr = -1;
    data.promoTc = -1;

    const char* className = "ChessGUI";
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = className;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClassA(&wc);

    hwnd = CreateWindowExA(0, className, "Chess - C++17",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        OFFSET_X*2 + 8*CELL, OFFSET_Y*2 + 8*CELL + 70,
        nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    while(GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
