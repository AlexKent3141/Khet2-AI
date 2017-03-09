#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <cstdint>
#include <string>

typedef uint8_t Square;

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

// The player types.
enum class Player
{
    Silver = 0,
    Red = 1
};

// The piece types.
enum class Piece
{
    None = 1,
    Anubis = 2,
    Pyramid = 3,
    Scarab = 4,
    Pharaoh = 5,
    Sphinx = 6
};

// The orientation of a piece.
enum class Orientation
{
    NE = 1,
    SE = 2,
    SW = 3,
    NW = 4
};

// Types of rotation.
enum class Rotation
{
    ACW = 1,
    CW = -1
};

// Movement directions.
enum class Direction
{
    N = BoardHeight,
    S = -BoardHeight,
    E = 1,
    W = -1,
    NE = N + E,
    SE = S + E,
    SW = S + W,
    NW = N + W
};

#endif // __GLOBALS_H__
