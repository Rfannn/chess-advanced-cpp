#ifndef GUI_H
#define GUI_H

//win32 gui - has to be before everything else or byte conflicts happen
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef min
#undef max

#include "Game.h"
#include <string>
using namespace std;

//shared state between wndproc and the class
struct GUIData {
    Game* game;
    int selRow;
    int selCol;
    string status;
    bool waitingPromo;
    int promoTr;
    int promoTc;
};

class GUI {
private:
    static GUIData data;
    static HWND hwnd;
    static HWND promoDlg;
    static HFONT pieceFont;
    static HFONT smallFont;
    static HFONT coordFont;
    static const int CELL = 64;
    static const int OFFSET_X = 50;
    static const int OFFSET_Y = 50;

    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    static LRESULT CALLBACK PromoProc(HWND, UINT, WPARAM, LPARAM);
    static void drawBoard(HDC hdc);
    static void drawSquareBorder(HDC hdc, int x, int y);
    static void handleClick(int x, int y);
    static void doMove(int fr, int fc, int tr, int tc);
    static void showPromotionDialog();

public:
    static void run(Game& g);
};

#endif
