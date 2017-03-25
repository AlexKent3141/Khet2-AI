#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "Types.h"
#include <string>
#include <vector>

// The padded dimensions of the board.
const int BoardWidth = 12;
const int BoardHeight = 10;
const int BoardArea = BoardWidth * BoardHeight;
const int NumPiecesPerPlayer = 13;

// Rules constants.
const int RepetitionLimit = 3;
const int TimeSinceCaptureLimit = 50;

// Can compute an upper-bound for the maximum game length.
// Assume that one piece is captured per TimeSinceCaptureLimit of moves.
const int MaxGameLength = TimeSinceCaptureLimit * 2 * (NumPiecesPerPlayer - 4);

// This constant is used to indicate that a location is off board.
const Square OffBoard = 0;

// This constant is used to indicate that a location is empty.
const Square Empty = 1;

// The standard opening configuration for the game.
const std::string Standard = 
    "x33a3ka3p22/2p37/3P46/p11P31s1s21p21P4/p21P41S2S11p11P3/6p23/7P12/2P4A1KA13X1 0";

// Movement directions.
const std::vector<int> Directions = 
{
    BoardWidth, 1, -BoardWidth, -1,                                  /* Orthogonals */
    BoardWidth + 1, BoardWidth - 1, -BoardWidth + 1, -BoardWidth - 1 /* Diagonals */
};

const int Absorbed = -2;

// Rotation directions.
const std::vector<int> Rotations = {
    1,  /* Clockwise */
    -1, /* Anti-Clockwise */
};

// Cache the Sphinx locations for each player.
const int Sphinx[2] = { 22, 97 };

// Cache the locations that each player can move to.
const int CanMove[2][BoardArea] = { 
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0,
        0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
        0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }
};

// Reflections map indicating the resulting laser direction given:
// 1). Incoming direction (index)
// 2). Piece type (-2)
// 3). Piece orientation (-1)
// A value of zero = death
const int Reflections[4][5][4] = {
    {
        { 0, 0, Absorbed, 0 },
        { 0, Directions[1], Directions[3], 0 },
        { Directions[3], Directions[1], Directions[3], Directions[1] },
        { 0, 0, 0, 0},
        { Absorbed, Absorbed, Absorbed, Absorbed }
    },
    {
        { 0, 0, 0, Absorbed },
        { 0, 0, Directions[2], Directions[0] },
        { Directions[2], Directions[0], Directions[2], Directions[0] },
        { 0, 0, 0, 0},
        { Absorbed, Absorbed, Absorbed, Absorbed }
    },
    {
        { Absorbed, 0, 0, 0 },
        { Directions[1], 0, 0, Directions[3] },
        { Directions[1], Directions[3], Directions[1], Directions[3] },
        { 0, 0, 0, 0},
        { Absorbed, Absorbed, Absorbed, Absorbed }
    },
    {
        { 0, Absorbed, 0, 0 },
        { Directions[0], Directions[2], 0, 0 },
        { Directions[0], Directions[2], Directions[0], Directions[2] },
        { 0, 0, 0, 0},
        { Absorbed, Absorbed, Absorbed, Absorbed }
    }
};

#endif // __GLOBALS_H__
