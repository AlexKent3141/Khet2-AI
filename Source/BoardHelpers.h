#include "Globals.h"

// The format for a Square (in bits) is:
// B1: Reserved.
// B2: Player
// B3-B5: Piece type
// B6-B8: Orientation

inline Square MakeBox(Player player, Piece piece, Orientation orientation)
{
    return (Square)((int)player << 1 | (int)piece << 2 | (int)orientation << 5);
}

inline Player GetOwner(Square s)
{
    return (Player)(s >> 1 & 0x1);
}

inline Piece GetPiece(Square s)
{
    return (Piece)(s >> 2 & 0x7);
}

inline Orientation GetOrientation(Square s)
{
    return (Orientation)(s >> 5 & 0x7);
}

inline bool IsPiece(Square s)
{
    return s != Empty && s != OffBoard;
}
