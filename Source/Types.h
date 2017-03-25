#ifndef __TYPES_H__
#define __TYPES_H__

#include <cstdint>

typedef uint8_t Square;

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
enum Orientation
{
    Up = 1,
    Right = 2,
    Down = 3,
    Left = 4
};

#endif // __TYPES_H__
