#include "Game.h"
#include "GUI.h"
#include <windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Game game;
    GUI::run(game);
    return 0;
}
