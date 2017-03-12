#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "Types.h"
#include <string>
#include <vector>

// The padded dimensions of the board.
const int BoardWidth = 12;
const int BoardHeight = 10;
const int BoardArea = BoardWidth * BoardHeight;

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
    BoardHeight, -BoardHeight, 1, -1,                                    /* Orthogonals */
    BoardHeight + 1, BoardHeight - 1, -BoardHeight + 1, -BoardHeight - 1 /* Diagonals */
};

// Rotation directions.
const std::vector<int> Rotations = {
    1,  /* Anti-clockwise */
    -1, /* Clockwise */
};

#endif // __GLOBALS_H__
