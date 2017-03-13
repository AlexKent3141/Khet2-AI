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
enum class Orientation
{
    NE = 1,
    SE = 2,
    SW = 3,
    NW = 4
};

// This type describes a move.
struct Move
{
    int Start;
    int End;
    int Rotation;

    Move(int start, int end, int rotation) : Start(start), End(end), Rotation(rotation)
    {
    }
};

#endif // __TYPES_H__
