#ifndef __BOARD_HELPERS_H__
#define __BOARD_HELPERS_H__

#include "Globals.h"

// The format for a Square (in bits) is:
// B1: Reserved.
// B2: Player
// B3-B5: Piece type
// B6-B8: Orientation

Square MakeBox(Player player, Piece piece, Orientation orientation)
{
    return (Square)((int)player << 1 | (int)piece << 2 | (int)orientation << 5);
}

Player GetPlayer(Square b)
{
    return (Player)(b >> 1 & 0x1);
}

Piece GetPiece(Square b)
{
    return (Piece)(b >> 2 & 0x7);
}

Orientation GetOrientation(Square b)
{
    return (Orientation)(b >> 5 & 0x7);
}

#endif // __BOARD_HELPERS_H__

