# advanced-cpp-chess

console chess game for two players. made with c++17 for a uni project.

## what works
- all pieces move correctly (i hope)
- check detection, checkmate and stalemate
- pawn promotion (ask for queen/rook/bishop/knight)
- castling works (kingside and queenside)
- en passant capture (was annoying to get right)
- save/load game to a text file
- no memory leaks (shared_ptr everywhere)

## build
```bash
g++ -std=c++17 -Wall -Wextra -I include src/*.cpp -o chess
```

## how to play
run the program, then type moves like:
```
e2 e4
```
or just:
```
e2e4
```
type `save` to save the game, `load` to load it back.

## files
- `common.h`: Position struct and Color/PieceType enums
- `Piece.h/cpp`: piece classes with isValidMove for each type
- `Board.h/cpp`: 8x8 grid, setup, printing
- `Game.h/cpp`: game loop, turn handling, rule checks
- `Renderer.h/cpp`: board display

## struggles
checkmate detection was the hardest part because you have to check every single legal move to see if any of them gets you out of check. en passant was also really confusing with the diagonal capture to an empty square and the timing of when it's allowed. castling took a while too because of all the conditions (can't be in check, squares must be empty, king/rook haven't moved).

there are probably still some bugs but it works for normal games.

## note
this was done step by step over a few days, not all at once. some parts are cleaner than others.
