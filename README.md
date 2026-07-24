# advanced-cpp-chess

a chess game in c++17 with both console and gui versions. made for a uni project.

## features
- all 6 pieces with correct movement
- check, checkmate, stalemate detection
- castling (kingside and queenside, with check-pass-through validation)
- en passant capture (tracked properly - only on the next move after a double push)
- pawn promotion (queen/rook/bishop/knight)
- save/load to a text file
- smart pointers everywhere (no memory leaks)
- gui version with win32 api (click to move, move hints shown)

## building

### console version
```bash
g++ -std=c++17 -Wall -Wextra -I include src/common.cpp src/Piece.cpp src/Board.cpp src/Renderer.cpp src/Game.cpp src/main.cpp -o chess
```

### gui version (windows only)
```bash
g++ -std=c++17 -Wall -Wextra -mwindows -I include src/common.cpp src/Piece.cpp src/Board.cpp src/Renderer.cpp src/Game.cpp src/GUI.cpp src/main_gui.cpp -o chess_gui -lgdi32 -luser32
```

### cmake
```bash
mkdir build && cd build
cmake ..
cmake --build .
```
this builds both `chess` (console) and `chess_gui` (gui) targets.

## how to play

### console
type moves in algebraic notation:
```
e2 e4
```
or without space:
```
e2e4
```
commands: `save`, `load`, `exit`

### gui
run chess_gui.exe. click a piece to select it (green highlight). green dots show legal moves, red dots show captures. click a destination to move. when a pawn reaches the last rank, a popup lets you choose the promotion piece.

## architecture

```
Piece (abstract base)
  ├── Pawn
  ├── Knight
  ├── Bishop
  ├── Rook
  ├── Queen
  └── King

Board       - 8x8 grid, holds shared_ptr<Piece>
Game        - game loop, rules, turn control, check detection
Renderer    - console display
GUI         - win32 window, mouse input, drawing
```

### design choices
- **shared_ptr**: all pieces stored as `shared_ptr<Piece>`. no raw new/delete.
- **virtual isValidMove**: each piece type implements its own movement logic.
- **separation**: game logic (Game) is separate from display (Renderer/GUI). the gui just calls `makeMove()` on the Game object.
- **MoveResult enum**: `makeMove()` returns a result code so the gui knows if it was legal, check, checkmate, promotion, etc.

## files
| file | description |
|------|-------------|
| `common.h` | Position struct, Color and PieceType enums |
| `Piece.h` | abstract Piece class + 6 concrete piece classes |
| `Piece.cpp` | isValidMove implementation for each piece |
| `Board.h/cpp` | 8x8 grid, piece placement, board setup |
| `Game.h/cpp` | game loop, move validation, check/checkmate/stalemate, save/load |
| `Renderer.h/cpp` | console board display with coordinates |
| `GUI.h/cpp` | win32 gui: window, drawing, mouse handling |
| `main.cpp` | console entry point |
| `main_gui.cpp` | gui entry point (WinMain) |

## save file format
```
<turn: 0=white, 1=black>
<for each square: type color moved>
<enPassantRow enPassantCol>
```
type: -1=empty, 0=Pawn, 1=Knight, 2=Bishop, 3=Rook, 4=Queen, 5=King
color: 0=White, 1=Black
moved: 0=no, 1=yes

## struggles
- **checkmate detection**: had to check every legal move for every piece. used `hasAnyMove()` which brute-forces all 64x64 squares.
- **en passant**: the timing was confusing. you can only capture en passant on the very next move after a double pawn push. added `enPassantTarget` to track this.
- **castling**: lots of conditions. king and rook can't have moved, squares between must be empty, king can't pass through check. added `squareAttacked()` helper.
- **win32 byte conflict**: `windows.h` has a `byte` typedef that conflicts with `std::byte`. fixed with `WIN32_LEAN_AND_MEAN` and `#undef min/max`.

## what's not perfect
- `safeMove()` does a shallow copy of the board (shared_ptr copy). works for check detection but technically not a deep copy.
- castling doesn't use the same code path as normal moves (intercepted before isValidMove).
- gui could use drag-and-drop instead of click-click.
