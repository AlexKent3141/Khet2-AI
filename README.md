# Khet2-AI
AI program which plays Khet 2.0. ([It's Chess, with frickin' laser beams!](https://www.youtube.com/watch?v=dUGPyvyUxnM))

Khet 2.0 is an interesting two-player board game where each player tries to position their pieces (some of which have reflective sides) so that when they fire their laser the opponent's Pharaoh is hit.

At the time of writing there isn't much established theory for Khet 2.0 so this work will be fairly experimental.

## Rules
The rules for Khet 2.0 can be found on the official website: http://khet.com/.

For the purposes of this project I will make the following restrictions and extensions:
1) The Pharaoh cannot rotate: this seems like a sensible restriction as in general this would not change the board state. Even if the board state would change (a piece will be "lasered" passively) I expect there to always be a more productive move available.
2) Scarabs can only rotate in one direction (clockwise): both Scarab rotation moves are equivalent, so it makes sense to restrict to one of them.
3) Threefold repetition: as defined in chess, if the same position is repeated three times then the game is drawn.
4) Fifty move rule: similar to the chess rule, if no pieces are captured in fifty moves then the game is drawn.

These may be subject to further tweaks once the AI can be tested fully.

## Building
The program is compiled using the GNU compiler and is built using the Unix *make* utility. At this point there is no build procedure for Windows.

## Usage
This program is now in a working state and can be used to analyse Khet 2.0 positions and suggest moves. Communications with the engine follow a protocol similar to the UCI protocol for chess (see "protocol.txt" for a full specification).

Example commands and responses:
```
./khet
position standard
go infinite
>info time 5 depth 1 score 80
>info time 55 depth 2 score 0
>info time 403 depth 3 score 0
>info time 3050 depth 4 score 0
>info time 6325 depth 5 score 40
stop
>bestmove h2g1
position standard h2g1
go movetime 10000
>info time 4 depth 1 score -40
>info time 39 depth 2 score 0
>info time 388 depth 3 score 40
>info time 3700 depth 4 score 40
>bestmove a5a6
quit
```

There is no graphical UI available at the moment (this may be a future project!). If you have a physical board then it is possible to use the "runner.py" script and manually relay the moves you make to the program.
